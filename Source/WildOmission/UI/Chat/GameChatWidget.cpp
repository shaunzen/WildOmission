// Copyright Telephone Studios. All Rights Reserved.


#include "GameChatWidget.h"
#include "ChatMessageWidget.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "WildOmission/Core/WildOmissionGameState.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"

UGameChatWidget::UGameChatWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UChatMessageWidget> ChatMessageBlueprint(TEXT("/Game/WildOmission/UI/Chat/WBP_ChatMessage"));
	if (ChatMessageBlueprint.Succeeded())
	{
		ChatMessageClass = ChatMessageBlueprint.Class;
	}

	Opened = false;
}

void UGameChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MessageContainer->ClearChildren();
	Close();
	MessageBox->OnTextCommitted.AddDynamic(this, &UGameChatWidget::OnMessageBoxTextCommitted);
	SendMessageButton->OnClicked.AddDynamic(this, &UGameChatWidget::AttemptSendMessage);
	AWildOmissionGameState* GameState = Cast<AWildOmissionGameState>(GetWorld()->GetGameState());
	if (GameState == nullptr)
	{
		return;
	}
	GameState->OnNewMessage.AddDynamic(this, &UGameChatWidget::RefreshMessages);
}

void UGameChatWidget::RefreshMessages()
{
	MessageContainer->ClearChildren();

	AWildOmissionGameState* GameState = Cast<AWildOmissionGameState>(GetWorld()->GetGameState());
	APlayerState* OwnerPlayerState = GetOwningPlayerState();
	if (GameState == nullptr || OwnerPlayerState == nullptr)
	{
		return;
	}

	FString OurPlayerNetName = OwnerPlayerState->GetPlayerName();

	TArray<FChatMessage> Messages;
	GameState->GetChatMessages(Messages);

	for (const FChatMessage& Message : Messages)
	{
		if (Message.ConnectionUpdate == true && Message.SenderName == OurPlayerNetName)
		{
			continue;
		}

		UChatMessageWidget* MessageWidget = CreateWidget<UChatMessageWidget>(this, ChatMessageClass);
		if (MessageWidget == nullptr)
		{
			continue;
		}

		MessageWidget->Setup(this, Message.SenderName, Message.Message, Message.TimeSent);

		MessageContainer->AddChild(MessageWidget);
	}
}

void UGameChatWidget::Open(UPlayerHUDWidget* InParentHUD)
{
	ParentHUD = InParentHUD;
	Opened = true;
	MessagePanel->SetVisibility(ESlateVisibility::Visible);
	MessageBox->SetFocus();
	
	// Show all old messages
	for (UWidget* Widget : MessageContainer->GetAllChildren())
	{
		Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGameChatWidget::Close()
{
	Opened = false;
	MessagePanel->SetVisibility(ESlateVisibility::Collapsed);
}

bool UGameChatWidget::IsOpen() const
{
	return Opened;
}

void UGameChatWidget::OnMessageBoxTextCommitted(const FText& MessageBoxText, ETextCommit::Type CommitMethod)
{
	AttemptSendMessage();
}

void UGameChatWidget::AttemptSendMessage()
{
	// Return if no message was typed
	if (MessageBox->GetText().ToString().Len() == 0 || ParentHUD == nullptr || GetOwningPlayer() == nullptr || GetOwningPlayerState() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot send nothing in game chat."));
		return;
	}

	AWildOmissionPlayerController* PlayerController = Cast<AWildOmissionPlayerController>(GetOwningPlayer());
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->Server_SendChatMessage(GetOwningPlayerState(), MessageBox->GetText().ToString());

	MessageBox->SetText(FText());
	ParentHUD->ToggleChatMenu();
}