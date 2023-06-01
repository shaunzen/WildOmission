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
class UCraftingRecipeCategoryButton;
class URecipeIconWidget;
class UIngredientRowWidget;
class UInventoryComponent;

enum EItemCategory;
struct FCraftingRecipe;

USTRUCT()
struct FCraftingRecipeEntry
{
	GENERATED_BODY()
	
	FName Name;
	int32 SortPriority;
	bool CanCraft;

};

UCLASS()
class WILDOMISSION_API UCraftingMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UCraftingMenuWidget(const FObjectInitializer& ObjectInializer);
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Refresh();

	void SetCategoryFilter(TEnumAsByte<EItemCategory> NewCategory);

	void SetSelectedRecipe(const FName& SelectedRecipeName);
	FName GetSelectedRecipe() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UCraftingRecipeCategoryButton* SortByButton_All;
	UPROPERTY(Meta = (BindWidget))
	UCraftingRecipeCategoryButton* SortByButton_Items;
	UPROPERTY(Meta = (BindWidget))
	UCraftingRecipeCategoryButton* SortByButton_Resources;
	UPROPERTY(Meta = (BindWidget))
	UCraftingRecipeCategoryButton* SortByButton_Tools;
	UPROPERTY(Meta = (BindWidget))
	UCraftingRecipeCategoryButton* SortByButton_Building;
	UPROPERTY(Meta = (BindWidget))
	UCraftingRecipeCategoryButton* SortByButton_Weapons;
	UPROPERTY(Meta = (BindWidget))
	UCraftingRecipeCategoryButton* SortByButton_Ammo;
	UPROPERTY(Meta = (BindWidget))
	UCraftingRecipeCategoryButton* SortByButton_Medical;
	UPROPERTY(Meta = (BindWidget))
	UCraftingRecipeCategoryButton* SortByButton_Clothing;

	UPROPERTY(Meta = (BindWidget))
	UWrapBox* RecipesWrapBox;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SelectedRecipeNameTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	UImage* SelectedRecipeIconImage;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* SelectedRecipeYieldTextBlock;

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

	TEnumAsByte<EItemCategory> CategoryFilter;

	FName SelectedRecipe;

	void RefreshRecipesList();
	bool RecipeSortPredicate(FName RecipeA, FName RecipeB);

	void RefreshDetailsPanel();
	void ClearDetailsPanel();
	void RefreshIngredientList();

	bool CanCraftRecipe(const FName& RecipeName);

	UFUNCTION()
	void Craft();

};
