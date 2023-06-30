// Copyright Telephone Studios. All Rights Reserved.


#include "ChatMessageWidget.h"
#include "GameChatWidget.h"
#include "Components/TextBlock.h"

void UChatMessageWidget::Setup(UGameChatWidget* InParent, const FString& PlayerName, const FString& Message, const double& InTimeSent)
{
	ParentChatWidget = InParent;
	FString PlayerNameString = FString::Printf(TEXT("%s: "), *PlayerName);
	PlayerNameText->SetText(FText::FromString(PlayerNameString));
	MessageText->SetText(FText::FromString(Message));
	TimeSent = InTimeSent;
}

void UChatMessageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (ParentChatWidget == nullptr)
	{
		return;
	}

	double CurrentTime = GetWorld()->GetRealTimeSeconds();
	double SecondsSinceMessageSent = CurrentTime - TimeSent;

	if (ParentChatWidget->IsOpen() || SecondsSinceMessageSent <= 5.0f)
	{
		return;
	}
	
	SetVisibility(ESlateVisibility::Hidden);
}