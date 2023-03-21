// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingMenuWidget.generated.h"

class UWrapBox;
class UTextBlock;
class UImage;
class UVerticalBox;
class UButton;
class URecipeIconWidget;
class UIngredientRowWidget;
class UInventoryComponent;

struct FCraftingRecipe;

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
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SelectedRecipeNameTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	UImage* SelectedRecipeIconImage;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SelectedRecipeDescriptionTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* IngredientListBox;

	UPROPERTY(Meta = (BindWidget))
	UButton* CraftButton;

	UPROPERTY()
	TSubclassOf<URecipeIconWidget> RecipeIconWidgetClass;
	
	UPROPERTY()
	TSubclassOf<UIngredientRowWidget> IngredientRowWidgetClass;

	FName SelectedRecipe;

	void UpdateSelectedRecipeDetailsPanel();

	void UpdateIngredientList(FCraftingRecipe* RecipeData, UInventoryComponent* OwnerInventoryComponent);

	bool CanCraftSelectedRecipe();

	UFUNCTION()
	void Craft();

};
