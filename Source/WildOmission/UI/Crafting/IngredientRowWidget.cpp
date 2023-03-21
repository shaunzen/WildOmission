// Copyright Telephone Studios. All Rights Reserved.


#include "IngredientRowWidget.h"
#include "Components/TextBlock.h"

void UIngredientRowWidget::Setup(const FName& IngredientName, const int32& IngredientAmountNeeded, const int32& IngredientHasAmount)
{
	if (IngredientNameTextBlock == nullptr
		|| AmountNeededTextBlock == nullptr
		|| HaveAmountTextBlock == nullptr)
	{
		return;
	}

	FString AmountNeededString = FString::Printf(TEXT("%i"), IngredientAmountNeeded);
	FString HaveAmountString = FString::Printf(TEXT("%i"), IngredientHasAmount);
	bHasEnough = IngredientAmountNeeded < IngredientHasAmount;

	IngredientNameTextBlock->SetText(FText::FromName(IngredientName));
	AmountNeededTextBlock->SetText(FText::FromString(AmountNeededString));
	HaveAmountTextBlock->SetText(FText::FromString(HaveAmountString));
}

bool UIngredientRowWidget::HasEnough() const
{
	return bHasEnough;
}