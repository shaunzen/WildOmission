// Copyright Telephone Studios. All Rights Reserved.


#include "GameChatWidget.h"
#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"

UGameChatWidget::UGameChatWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UChatMessageWidget> ChatMessageBlueprint(TEXT("/Game/WildOmission/UI/Chat/WBP_ChatMessage"));
	if (ChatMessageBlueprint.Succeeded())
	{
		ChatMessageClass = ChatMessageBlueprint.Class;
	}
}

void UGameChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SendMessageButton->OnClicked.AddDynamic(this, &UGameChatWidget::SendMessage);
}

void UGameChatWidget::RefreshMessages()
{

}

void UGameChatWidget::SendMessage()
{
	// if no message was typed, return
}