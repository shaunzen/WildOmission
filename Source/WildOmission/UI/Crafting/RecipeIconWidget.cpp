// Copyright Telephone Studios. All Rights Reserved.


#include "RecipeIconWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "CraftingMenuWidget.h"
#include "WildOmission/Core/Structs/ItemData.h"
#include "WildOmission/Core/WildOmissionStatics.h"

void URecipeIconWidget::Setup(UCraftingMenuWidget* InParentMenu, const FCraftingRecipeEntry& InParentEntry)
{
	ParentMenu = InParentMenu;
	ParentEntry = InParentEntry;

	RecipeIconBorder->SetBrushFromMaterial(ParentEntry.YieldItemData->Thumbnail);

	RecipeButton->OnClicked.AddDynamic(this, &URecipeIconWidget::OnClicked);
}

bool URecipeIconWidget::IsSelected() const
{
	if (ParentMenu == nullptr)
	{
		return false;
	}

	return ParentMenu->GetSelectedRecipe() == ParentEntry.RecipeID;
}

bool URecipeIconWidget::IsCraftable() const
{
	return ParentEntry.IngredientPercentage == 100;
}

void URecipeIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}

void URecipeIconWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Hovering = true;
}

void URecipeIconWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Hovering = false;
}

void URecipeIconWidget::OnClicked()
{
	ParentMenu->SetSelectedRecipe(ParentEntry.RecipeID);
}