// Copyright Telephone Studios. All Rights Reserved.


#include "UI/GameChatWidget.h"
#include "ChatMessageWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Interfaces/GameChatParentWidget.h"
#include "Interfaces/ChatMessageContainer.h"
#include "Interfaces/ChatMessageSender.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"

UGameChatWidget::UGameChatWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UChatMessageWidget> ChatMessageBlueprint(TEXT("/Game/GameChat/UI/WBP_ChatMessage"));
	if (ChatMessageBlueprint.Succeeded())
	{
		ChatMessageClass = ChatMessageBlueprint.Class;
	}

	Opened = false;
}

void UGameChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MessageContainerPanel->ClearChildren();
	Close();
	MessageBox->OnTextCommitted.AddDynamic(this, &UGameChatWidget::OnMessageBoxTextCommitted);
	SendMessageButton->OnClicked.AddDynamic(this, &UGameChatWidget::AttemptSendMessage);
}

void UGameChatWidget::Setup(IGameChatParentWidget* InParentWidget, IChatMessageContainer* InMessageContainer)
{
	ParentWidget = InParentWidget;
	MessageContainer = InMessageContainer;
	if (MessageContainer == nullptr)
	{
		return;
	}
	MessageContainer->OnChatReplicated.AddDynamic(this, &UGameChatWidget::RefreshMessages);
}

void UGameChatWidget::RefreshMessages()
{
	MessageContainerPanel->ClearChildren();

	APlayerState* OwnerPlayerState = GetOwningPlayerState();
	if (MessageContainer == nullptr || OwnerPlayerState == nullptr)
	{
		return;
	}

	FString OurPlayerNetName = OwnerPlayerState->GetPlayerName();

	TArray<FChatMessage> Messages;
	MessageContainer->GetChatMessages(Messages);
	
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

		MessageWidget->Setup(this, Message.SenderName, Message.Message, Message.TimeSent);

		MessageContainerPanel->AddChild(MessageWidget);
	}
}

void UGameChatWidget::Open(IChatMessageSender* InOwnerMessageSender)
{
	OwnerMessageSender = InOwnerMessageSender;
	Opened = true;
	MessagePanel->SetVisibility(ESlateVisibility::Visible);
	MessageBox->SetFocus();
	
	// Show all old messages
	for (UWidget* Widget : MessageContainerPanel->GetAllChildren())
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
	if (MessageBox->GetText().ToString().Len() == 0 
		|| ParentWidget == nullptr
		|| OwnerMessageSender == nullptr 
		|| GetOwningPlayer() == nullptr 
		|| GetOwningPlayerState() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot send nothing in game chat."));
		return;
	}

	OwnerMessageSender->SendMessage(GetOwningPlayerState(), MessageBox->GetText().ToString());

	MessageBox->SetText(FText());
	ParentWidget->ToggleChatMenu();
}