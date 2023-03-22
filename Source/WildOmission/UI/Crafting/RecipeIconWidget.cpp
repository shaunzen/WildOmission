// Copyright Telephone Studios. All Rights Reserved.


#include "RecipeIconWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "WildOmission/Components/CraftingComponent.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "CraftingMenuWidget.h"

void URecipeIconWidget::Setup(UCraftingMenuWidget* InParent, const FName& InRecipeName, UMaterialInstance* Icon)
{
	Parent = InParent;
	RecipeName = InRecipeName;
	RecipeIconBorder->SetBrushFromMaterial(Icon);

	bCanCraft = EvaluateCraftibility();
	
	RecipeButton->OnClicked.AddDynamic(this, &URecipeIconWidget::OnClicked);
}

bool URecipeIconWidget::IsSelected() const
{
	if (Parent == nullptr)
	{
		return false;
	}

	return Parent->GetSelectedRecipe() == RecipeName;
}

bool URecipeIconWidget::IsCraftable() const
{
	return bCanCraft;
}

void URecipeIconWidget::OnClicked()
{
	Parent->SetSelectedRecipe(RecipeName);
}

bool URecipeIconWidget::EvaluateCraftibility()
{
	UCraftingComponent* OwnerCraftingComponent = GetOwningPlayerPawn()->FindComponentByClass<UCraftingComponent>();
	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	if (OwnerCraftingComponent == nullptr || OwnerInventoryComponent == nullptr)
	{
		return false;
	}

	FCraftingRecipe* RecipeData = OwnerCraftingComponent->GetRecipe(RecipeName);
	if (RecipeData == nullptr)
	{
		return false;
	}

	for (const FInventoryItem& Ingredient : RecipeData->Ingredients)
	{
		int32 PlayerHas = OwnerInventoryComponent->GetContents()->GetItemQuantity(Ingredient.Name);
		if (PlayerHas < Ingredient.Quantity)
		{
			return false;
		}
	}

	return true;
}