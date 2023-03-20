// Copyright Telephone Studios. All Rights Reserved.


#include "RecipeIconWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"

void URecipeIconWidget::Setup(UCraftingMenuWidget* InParent, const uint32& InIndex, UMaterialInstance* Icon)
{
	Parent = InParent;
	Index = InIndex;
	RecipeIconBorder->SetBrushFromMaterial(Icon);
	RecipeButton->OnClicked.AddDynamic(this, &URecipeIconWidget::OnClicked);
}

void URecipeIconWidget::OnClicked()
{
	// TODO
	//Parent->SetSelectedRecipe(Index);
}