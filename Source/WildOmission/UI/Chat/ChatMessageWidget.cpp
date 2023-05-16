// Copyright Telephone Studios. All Rights Reserved.


#include "ChatMessageWidget.h"
#include "Components/TextBlock.h"

void UChatMessageWidget::Setup(const FString& PlayerName, const FString& Message, const int32& Index)
{
	PlayerNameText->SetText(FText::FromString(PlayerName));
	MessageText->SetText(FText::FromString(Message));
	MessageIndex = Index;
}