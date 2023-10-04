// Copyright Telephone Studios. All Rights Reserved.


#include "UI/GameChatWidget.h"
#include "ChatMessageWidget.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/PlayerState.h"
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

	MessageContainerPanel->ClearChildren();
	
	Close();
	
	MessageBox->OnTextCommitted.AddDynamic(this, &UGameChatWidget::OnMessageBoxTextCommitted);
}

void UGameChatWidget::Setup()
{
	
	//MessageContainer->OnChatReplicated.AddDynamic(this, &UGameChatWidget::RefreshMessages);

	RefreshMessages();
}

void UGameChatWidget::RefreshMessages()
{
	MessageContainerPanel->ClearChildren();

	APlayerState* OwnerPlayerState = GetOwningPlayerState();
	if (OwnerPlayerState == nullptr)
	{
		return;
	}

	const FString OurPlayerNetName = OwnerPlayerState->GetPlayerName();

	TArray<FChatMessage> Messages;
	//MessageContainer->GetChatMessages(Messages);
	
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
	// Return if no message was typed
	if (MessageBox->GetText().ToString().Len() == 0 
		|| GetOwningPlayer() == nullptr 
		|| GetOwningPlayerState() == nullptr)
	{
		UE_LOG(LogGameChat, Display, TEXT("Cannot send nothing in game chat."));
		return;
	}

	
	MessageBox->SetText(FText());
}