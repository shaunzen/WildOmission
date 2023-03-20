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

		URecipeIconWidget* NewRecipe = CreateWidget<URecipeIconWidget>(this, RecipeIconWidgetClass);
		RecipesWrapBox->AddChild(NewRecipe);

		FItemData* YeildItemData = OwnerInventoryComponent->GetItemData(RecipeData->Yeild.Name);
		if (YeildItemData == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find item data for recipe yeild %s"), *RecipeData->Yeild.Name.ToString());
			return;
		}

		// Set recipe defaults
		NewRecipe->Setup(this, 0, YeildItemData->Thumbnail);
	}
	
	// needed
	// TSubclassOf pointer to the recipe icon class so we can instanciate them
	// pointer to the scroll box
}

void UCraftingMenuWidget::Craft()
{
	FName RecipeName("pickaxe");

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
	
	OwnerCraftingComponent->Server_CraftItem(RecipeName);
}