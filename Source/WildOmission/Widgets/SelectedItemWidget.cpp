// Copyright Telephone Studios. All Rights Reserved.


#include "SelectedItemWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void USelectedItemWidget::SetItem(UTexture2D* ItemIcon, int32 Quantity)
{
	FString QuantityString;
	if (Quantity > 1)
	{
		QuantityString = FString::Printf(TEXT("x%i"), Quantity);
	}
	else
	{
		QuantityString = FString("");
	}
	IconBorder->SetBrushFromTexture(ItemIcon);
	QuantityText->SetText(FText::FromString(QuantityString));
}

void USelectedItemWidget::Show()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USelectedItemWidget::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}