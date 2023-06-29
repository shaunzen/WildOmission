// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngredientRowWidget.generated.h"

class UTextBlock;

UCLASS()
class UIngredientRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(const FString& IngredientName, const int32& IngredientAmountNeeded, const int32& IngredientHasAmount);

	UFUNCTION(BlueprintCallable)
	bool HasEnough() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* IngredientNameTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* AmountNeededTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* HaveAmountTextBlock;

	bool bHasEnough;

	void SetColor();

};
