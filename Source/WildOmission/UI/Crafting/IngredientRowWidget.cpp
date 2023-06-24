// Copyright Telephone Studios. All Rights Reserved.


#include "IngredientRowWidget.h"
#include "Components/TextBlock.h"
#include "UIHelpers.h"

void UIngredientRowWidget::Setup(const FString& IngredientName, const int32& IngredientAmountNeeded, const int32& IngredientHasAmount)
{
	if (IngredientNameTextBlock == nullptr
		|| AmountNeededTextBlock == nullptr
		|| HaveAmountTextBlock == nullptr)
	{
		return;
	}

	FString AmountNeededString = FString::Printf(TEXT("%i"), IngredientAmountNeeded);
	FString HaveAmountString = FString::Printf(TEXT("%i"), IngredientHasAmount);
	bHasEnough = IngredientAmountNeeded <= IngredientHasAmount;

	IngredientNameTextBlock->SetText(FText::FromString(IngredientName));
	AmountNeededTextBlock->SetText(FText::FromString(AmountNeededString));
	HaveAmountTextBlock->SetText(FText::FromString(HaveAmountString));

	SetColor();
}

bool UIngredientRowWidget::HasEnough() const
{
	return bHasEnough;
}

void UIngredientRowWidget::SetColor()
{
	if (bHasEnough)
	{
		FUIColor* White = UUIHelpers::GetUIColor(FName("White"));
		SetColorAndOpacity(White->Default);
	}
	else
	{
		FUIColor* Red = UUIHelpers::GetUIColor(FName("Red"));
		SetColorAndOpacity(Red->Default);
	}
}