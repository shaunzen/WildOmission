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

void URecipeIconWidget::OnClicked()
{
	Parent->SetSelectedRecipe(RecipeName);
}