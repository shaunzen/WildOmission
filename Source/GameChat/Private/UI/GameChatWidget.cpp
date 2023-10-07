// Copyright Telephone Studios. All Rights Reserved.


#include "UI/GameChatWidget.h"
#include "ChatMessageWidget.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Interfaces/PlayerControllerMessageSender.h"
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
	AGameChatHandler* ChatHandler = AGameChatHandler::GetGameChatHandler();
	if (ChatHandler == nullptr)
	{
		UE_LOG(LogGameChat, Warning, TEXT("Couldn't retrieve game chat handler."));
		return;
	}

	ChatHandler->OnMessageRecieved.AddDynamic(this, &UGameChatWidget::RefreshMessages);
}

void UGameChatWidget::RefreshMessages()
{
	AGameChatHandler* ChatHandler = AGameChatHandler::GetGameChatHandler();
	APlayerState* OwnerPlayerState = GetOwningPlayerState();
	if (ChatHandler == nullptr || OwnerPlayerState == nullptr)
	{
		return;
	}
	
	MessageContainerPanel->ClearChildren();
	const FString OurPlayerNetName = OwnerPlayerState->GetPlayerName();

	TArray<FChatMessage> Messages;
	Messages = ChatHandler->GetChatMessages();

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

	IPlayerControllerMessageSender* MessageSender = Cast<IPlayerControllerMessageSender>(GetOwningPlayer());
	APlayerState* OwningPlayerState = GetOwningPlayerState();
	
	// Return if no message was typed
	if (MessageBoxEmpty || MessageSender == nullptr || OwningPlayerState == nullptr)
	{
		UE_LOG(LogGameChat, Display, TEXT("Cannot send nothing in game chat."));
		return;
	}

	MessageSender->SendMessage(OwningPlayerState, MessageBox->GetText().ToString());
	
	MessageBox->SetText(FText());
}