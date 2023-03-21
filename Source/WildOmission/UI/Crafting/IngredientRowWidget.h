// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngredientRowWidget.generated.h"

class UTextBlock;

UCLASS()
class WILDOMISSION_API UIngredientRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(const FName& IngredientName, const int32& IngredientAmountNeeded, const int32& IngredientHasAmount);

	UFUNCTION(BlueprintCallable)
	bool HasEnough() const;

	UFUNCTION(BlueprintImplementableEvent)
	void SetColor();

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* IngredientNameTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* AmountNeededTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* HaveAmountTextBlock;

	bool bHasEnough;

};
