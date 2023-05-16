// Copyright Telephone Studios. All Rights Reserved.


#include "GameChatWidget.h"
#include "ChatMessageWidget.h"
#include "Components/Button.h"
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

	Close();
	SendMessageButton->OnClicked.AddDynamic(this, &UGameChatWidget::SendMessage);
}

void UGameChatWidget::RefreshMessages()
{

}

void UGameChatWidget::Open()
{
	Opened = true;
	MessagePanel->SetVisibility(ESlateVisibility::Visible);
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

void UGameChatWidget::SendMessage()
{
	// if no message was typed, return
}