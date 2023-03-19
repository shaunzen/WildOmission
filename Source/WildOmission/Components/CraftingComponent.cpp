// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingComponent.h"
#include "InventoryComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	
	ConstructorHelpers::FObjectFinder<UDataTable> RecipeDataTableBlueprint(TEXT("/Game/WildOmission/Core/DataTables/DT_CraftingRecipes"));
	if (RecipeDataTableBlueprint.Object == nullptr)
	{
		return;
	}

	RecipeDataTable = RecipeDataTableBlueprint.Object;
}


// Called when the game starts
void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCraftingComponent::Server_CraftItem_Implementation(const FName& ItemToCraft)
{
	// find the recipe in the data table
	FCraftingRecipe* RecipeData = GetRecipe(ItemToCraft);
	if (RecipeData == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString::Printf(TEXT("Failed to find recipe data for recipe: %s"), *ItemToCraft.ToString()));
		return;
	}

	// get the owners inventory comp
	UInventoryComponent* OwnerInventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventoryComponent == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString("Failed to get owner inventory component"));
		return;
	}

	// check they have the ingredients
	for (const FInventoryItem& Ingredient : RecipeData->Ingredients)
	{
		int32 IngredientAmountInInventory = OwnerInventoryComponent->GetContents()->GetItemQuantity(Ingredient.Name);
		
		// return if they dont
		if (IngredientAmountInInventory < Ingredient.Quantity)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString::Printf(TEXT("Cannot craft %s: not enough %s, Needed %i, you have %i"), *ItemToCraft.ToString(), *Ingredient.Name.ToString(), Ingredient.Quantity, IngredientAmountInInventory));
			return;
		}
	}

	// remove ingredients
	for (const FInventoryItem& Ingredient : RecipeData->Ingredients)
	{
		OwnerInventoryComponent->RemoveItem(Ingredient);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Yellow, FString::Printf(TEXT("Removed Ingredient: Name: %s Quantity: %i"), *Ingredient.Name.ToString(), Ingredient.Quantity));
	}

	// add resulting yeild
	OwnerInventoryComponent->AddItem(RecipeData->Yeild);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString::Printf(TEXT("Crafted: Name: %s Quantity: %i"), *RecipeData->Yeild.Name.ToString(), RecipeData->Yeild.Quantity));
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