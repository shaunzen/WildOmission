// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingMenuWidget.generated.h"

class UWrapBox;
class URecipeIconWidget;

UCLASS()
class WILDOMISSION_API UCraftingMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UCraftingMenuWidget(const FObjectInitializer& ObjectInializer);

	void OnOpen();

	void SetSelectedRecipe(const FName& SelectedRecipeName);

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UWrapBox* RecipesWrapBox;
	
	UPROPERTY()
	TSubclassOf<URecipeIconWidget> RecipeIconWidgetClass;
	
	FName SelectedRecipe;

	void UpdateSelectedRecipeInfo();

	UFUNCTION()
	void Craft();

};
