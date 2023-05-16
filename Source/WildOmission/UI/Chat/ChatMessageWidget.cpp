// Copyright Telephone Studios. All Rights Reserved.


#include "ChatMessageWidget.h"
#include "GameChatWidget.h"
#include "Components/TextBlock.h"

void UChatMessageWidget::Setup(UGameChatWidget* InParent, const FString& PlayerName, const FString& Message, const double& InTimeSent)
{
	ParentChatWidget = InParent;
	PlayerNameText->SetText(FText::FromString(PlayerName));
	MessageText->SetText(FText::FromString(Message));
	TimeSent = InTimeSent;
}

void UChatMessageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// TODO determine show/hide state
}