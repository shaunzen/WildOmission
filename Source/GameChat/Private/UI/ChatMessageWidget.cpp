// Copyright Telephone Studios. All Rights Reserved.


#include "ChatMessageWidget.h"
#include "UI/GameChatWidget.h"
#include "Components/TextBlock.h"
#include "Structs/ChatMessage.h"
#include "Color/UIColors.h"

void UChatMessageWidget::Setup(UGameChatWidget* InParent, const FChatMessage& InChatMessage)
{
	ParentChatWidget = InParent;

	FString PlayerNameString = FString::Printf(TEXT("%s: "), *InChatMessage.SenderName);
	PlayerNameText->SetText(FText::FromString(PlayerNameString));

	MessageText->SetText(FText::FromString(InChatMessage.Message));
	TimeRecieved = InChatMessage.TimeRecieved;

	FUIColor* NameColor = InChatMessage.SenderIsAdminisrator ? UUIColors::GetBaseColor(TEXT("Blue")) : UUIColors::GetBaseColor(TEXT("Red"));
	if (NameColor == nullptr)
	{
		return;
	}

	PlayerNameText->SetColorAndOpacity(FSlateColor(NameColor->Default));
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

	if (ParentChatWidget->IsOpen() || SecondsSinceMessageRecieved <= 5.0f)
	{
		SetVisibility(ESlateVisibility::Visible);
		return;
	}
	
	SetVisibility(ESlateVisibility::Collapsed);
}