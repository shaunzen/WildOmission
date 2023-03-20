// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecipeIconWidget.generated.h"

class UButton;
class UBorder;
class UCraftingMenuWidget;

UCLASS()
class WILDOMISSION_API URecipeIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup(UCraftingMenuWidget* InParent, const uint32& InIndex, UMaterialInstance* Icon);

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* RecipeButton;
	
	UPROPERTY(Meta = (BindWidget))
	UBorder* RecipeIconBorder;

	UPROPERTY()
	UCraftingMenuWidget* Parent;

	uint32 Index;

	UFUNCTION()
	void OnClicked();

};
