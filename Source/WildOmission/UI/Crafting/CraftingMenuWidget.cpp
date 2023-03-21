// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingMenuWidget.h"
#include "Components/WrapBox.h"
#include "RecipeIconWidget.h"
#include "WildOmission/Components/CraftingComponent.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "UObject/ConstructorHelpers.h"

UCraftingMenuWidget::UCraftingMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<URecipeIconWidget> RecipeIconWidgetBlueprint(TEXT("/Game/WildOmission/UI/Crafting/WBP_RecipeIcon"));
	if (RecipeIconWidgetBlueprint.Class == nullptr)
	{
		return;
	}

	RecipeIconWidgetClass = RecipeIconWidgetBlueprint.Class;
}

bool UCraftingMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	
	if (Success == false
		|| RecipeIconWidgetClass == nullptr
		|| RecipesWrapBox == nullptr)
	{
		return false;
	}
	// Set entry
	return true;
}

void UCraftingMenuWidget::OnOpen()
{
	// Clear all children of the scroll box
	RecipesWrapBox->ClearChildren();

	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	UCraftingComponent* OwnerCraftingComponent = GetOwningPlayerPawn()->FindComponentByClass<UCraftingComponent>();
	if (OwnerInventoryComponent == nullptr || OwnerCraftingComponent == nullptr)
	{
		return;
	}

	// For each entry create a new recipe icon and populate it with the relavent information
	for (const FName& RecipeName : OwnerCraftingComponent->GetAllRecipes())
	{
		// Get recipe data table entries
		FCraftingRecipe* RecipeData = OwnerCraftingComponent->GetRecipe(RecipeName);
		if (RecipeData == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find recipe data for %s"), *RecipeName.ToString());
			return;
		}
		FItemData* YeildItemData = OwnerInventoryComponent->GetItemData(RecipeData->Yeild.Name);
		if (YeildItemData == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find item data for recipe yeild %s"), *RecipeData->Yeild.Name.ToString());
			return;
		}

		URecipeIconWidget* NewRecipe = CreateWidget<URecipeIconWidget>(this, RecipeIconWidgetClass);
		NewRecipe->Setup(this, RecipeName, YeildItemData->Thumbnail);
		RecipesWrapBox->AddChild(NewRecipe);
	}
}

void UCraftingMenuWidget::SetSelectedRecipe(const FName& SelectedRecipeName)
{
	SelectedRecipe = SelectedRecipeName;
	
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Orange, FString::Printf(TEXT("New Recipe Selected: %s"), *SelectedRecipe.ToString()));

	UpdateSelectedRecipeInfo();
}

void UCraftingMenuWidget::UpdateSelectedRecipeInfo()
{
	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	UCraftingComponent* OwnerCraftingComponent = GetOwningPlayerPawn()->FindComponentByClass<UCraftingComponent>();
	if (OwnerInventoryComponent == nullptr || OwnerCraftingComponent == nullptr)
	{
		return;
	}

	FCraftingRecipe* RecipeData = OwnerCraftingComponent->GetRecipe(SelectedRecipe);
	if (RecipeData == nullptr)
	{
		return;
	}

	for (const FInventoryItem& Ingredient : RecipeData->Ingredients)
	{
		// Create a new ingredient row
		// Populate it with the relevent information
		// Add to IngredientListBox
	}
}

void UCraftingMenuWidget::Craft()
{
	APawn* PawnOwner = GetOwningPlayerPawn<APawn>();
	if (PawnOwner == nullptr)
	{
		return;
	}

	UCraftingComponent* OwnerCraftingComponent = PawnOwner->FindComponentByClass<UCraftingComponent>();
	if (OwnerCraftingComponent == nullptr)
	{
		return;
	}
	
	OwnerCraftingComponent->Server_CraftItem(SelectedRecipe);
}