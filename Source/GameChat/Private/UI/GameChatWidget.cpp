// Copyright Telephone Studios. All Rights Reserved.


#include "UI/GameChatWidget.h"
#include "ChatMessageWidget.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/PlayerState.h"
#include "GameChatHandler.h"
#include "UObject/ConstructorHelpers.h"
#include "Structs/ChatMessage.h"
#include "Log.h"

UGameChatWidget::UGameChatWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	MessageContainerPanel = nullptr;
	MessagePanel = nullptr;
	MessageBox = nullptr;
	
	ChatMessageClass = nullptr;

	static ConstructorHelpers::FClassFinder<UChatMessageWidget> ChatMessageBlueprint(TEXT("/Game/GameChat/UI/WBP_ChatMessage"));
	if (ChatMessageBlueprint.Succeeded())
	{
		ChatMessageClass = ChatMessageBlueprint.Class;
	}

	Opened = false;
}

void UGameChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshMessages();
	MessageContainerPanel->ClearChildren();	
	Close();
	MessageBox->OnTextCommitted.AddDynamic(this, &UGameChatWidget::OnMessageBoxTextCommitted);
	AGameChatHandler* ChatHandler = AGameChatHandler::GetInstance();
	if (ChatHandler == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something went wrong finding chat handler."));
		return;
	}

	ChatHandler->OnMessageRecieved.AddDynamic(this, &UGameChatWidget::RefreshMessages);
}

void UGameChatWidget::RefreshMessages()
{
	UE_LOG(LogTemp, Warning, TEXT("Refreshing Messages Widget."));
	APlayerState* OwnerPlayerState = GetOwningPlayerState();
	if (OwnerPlayerState == nullptr)
	{
		return;
	}
	
	MessageContainerPanel->ClearChildren();
	const FString OurPlayerNetName = OwnerPlayerState->GetPlayerName();

	TArray<FChatMessage> Messages;
	Messages = AGameChatHandler::GetChatMessages();

	if (Messages.IsEmpty())
	{
		return;
	}

	for (int32 i = Messages.Num()-1; i > -1; --i)
	{
		const FChatMessage& Message = Messages[i];

		if (Message.ConnectionUpdate == true && Message.SenderName == OurPlayerNetName)
		{
			continue;
		}

		UChatMessageWidget* MessageWidget = CreateWidget<UChatMessageWidget>(this, ChatMessageClass);
		if (MessageWidget == nullptr)
		{
			continue;
		}

		MessageWidget->Setup(this, Message.SenderName, Message.Message, Message.TimeRecieved);

		MessageContainerPanel->AddChild(MessageWidget);
	}
}

void UGameChatWidget::Open()
{
	Opened = true;
	MessagePanel->SetVisibility(ESlateVisibility::Visible);
	MessageBox->SetFocus();
	
	// Show all old messages
	for (UWidget* Widget : MessageContainerPanel->GetAllChildren())
	{
		Widget->SetVisibility(ESlateVisibility::Visible);
	}

	MessageContainerPanel->ScrollToEnd();
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
	if (CommitMethod == ETextCommit::Type::OnEnter)
	{
		AttemptSendMessage();
	}
	else if (CommitMethod != ETextCommit::Type::OnEnter && OnToggleRequested.IsBound())
	{
		OnToggleRequested.Broadcast();
	}
}

void UGameChatWidget::AttemptSendMessage()
{
	const bool MessageBoxEmpty = MessageBox->GetText().ToString().Len() == 0;
	APlayerState* OwningPlayerState = GetOwningPlayerState();
	AGameChatHandler* ChatHandler = AGameChatHandler::GetInstance();
	
	// Return if no message was typed
	if (MessageBoxEmpty || OwningPlayerState == nullptr || ChatHandler == nullptr)
	{
		UE_LOG(LogGameChat, Display, TEXT("Cannot send nothing in game chat."));
		return;
	}

	ChatHandler->Server_SendMessage(OwningPlayerState, MessageBox->GetText().ToString());
	
	MessageBox->SetText(FText());
}