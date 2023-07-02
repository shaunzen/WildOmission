// Copyright Telephone Studios. All Rights Reserved.


#include "Components/CraftingComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static UDataTable* RecipeDataTable = nullptr;

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> RecipeDataTableBlueprint(TEXT("/Game/WildOmission/Core/DataTables/DT_CraftingRecipes"));
	if (RecipeDataTableBlueprint.Succeeded())
	{
		RecipeDataTable = RecipeDataTableBlueprint.Object;
	}
}

void UCraftingComponent::Server_CraftItem_Implementation(const FName& ItemToCraft)
{
	FCraftingRecipe* RecipeData = GetRecipe(ItemToCraft);
	if (RecipeData == nullptr)
	{
		return;
	}

	UInventoryComponent* OwnerInventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventoryComponent == nullptr)
	{
		return;
	}

	for (const FInventoryItem& Ingredient : RecipeData->Ingredients)
	{
		int32 IngredientAmountInInventory = OwnerInventoryComponent->GetContents()->GetItemQuantity(Ingredient.Name);
		
		if (IngredientAmountInInventory < Ingredient.Quantity)
		{
			return;
		}
	}

	for (const FInventoryItem& Ingredient : RecipeData->Ingredients)
	{
		OwnerInventoryComponent->RemoveItem(Ingredient);
	}

	FInventoryItem Yield;
	Yield.Name = ItemToCraft;
	Yield.Quantity = RecipeData->YieldQuantity;

	OwnerInventoryComponent->AddItem(Yield);
}

TArray<FName> UCraftingComponent::GetAllRecipes()
{
	if (RecipeDataTable == nullptr)
	{
		UE_LOG(LogCrafting, Display, TEXT("Cannot get all crafting recipes, the recipe data table is missing."));
		TArray<FName> OutEmptyList;
		return OutEmptyList;
	}

	return RecipeDataTable->GetRowNames();
}

FCraftingRecipe* UCraftingComponent::GetRecipe(const FName& RecipeName)
{
	if (RecipeDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Recipe Context"));
	
	return RecipeDataTable->FindRow<FCraftingRecipe>(RecipeName, ContextString);
}