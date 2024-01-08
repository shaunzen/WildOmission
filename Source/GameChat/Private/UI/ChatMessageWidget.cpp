// Copyright Telephone Studios. All Rights Reserved.


#include "ChatMessageWidget.h"
#include "UI/GameChatWidget.h"
#include "Components/TextBlock.h"

void UChatMessageWidget::Setup(UGameChatWidget* InParent, const FString& PlayerName, const FString& Message, const double& InTimeRecieved)
{
	ParentChatWidget = InParent;
	FString PlayerNameString = FString::Printf(TEXT("%s: "), *PlayerName);
	PlayerNameText->SetText(FText::FromString(PlayerNameString));
	MessageText->SetText(FText::FromString(Message));
	TimeRecieved = InTimeRecieved;
}

void UChatMessageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UWorld* World = GetWorld();
	if (World == nullptr || ParentChatWidget == nullptr)
	{
		return;
	}

	double CurrentTime = World->GetRealTimeSeconds();
	double SecondsSinceMessageRecieved = CurrentTime - TimeRecieved;

	if (ParentChatWidget->IsOpen() || (!ParentChatWidget->GetHideUnlessOpen() && SecondsSinceMessageRecieved <= 5.0f))
	{
		SetVisibility(ESlateVisibility::Visible);
		return;
	}
	
	SetVisibility(ESlateVisibility::Collapsed);
}