// Copyright Telephone Studios. All Rights Reserved.


#include "RecipeIconWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "CraftingMenuWidget.h"

void URecipeIconWidget::Setup(UCraftingMenuWidget* InParent, const FName& InRecipeName, UMaterialInstance* Icon)
{
	Parent = InParent;
	RecipeName = InRecipeName;
	RecipeIconBorder->SetBrushFromMaterial(Icon);

	// todo cancraft recipe
	bCanCraft = true;
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