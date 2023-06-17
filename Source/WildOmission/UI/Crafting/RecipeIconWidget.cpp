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
	return ParentEntry.IngredientPercentage >= 100;
}

void URecipeIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (IsSelected())
	{
		FUIColor* Blue = UWildOmissionStatics::GetUIColor(FName("Blue"));
		if (Hovering)
		{
			BackgroundBorder->SetBrushColor(UWildOmissionStatics::GetHighlightedColor(Blue) - FLinearColor(0.0f, 0.0f, 0.0f, 0.7f));
		}
		else
		{
			BackgroundBorder->SetBrushColor(Blue->Default - FLinearColor(0.0f, 0.0f, 0.0f, 0.7f));
		}
	}
	else
	{
		FUIColor* White = UWildOmissionStatics::GetUIColor(FName("White"));

		if (Hovering)
		{
			BackgroundBorder->SetBrushColor(UWildOmissionStatics::GetHighlightedColor(White) - FLinearColor(0.0f, 0.0f, 0.0f, 0.7f));
		}
		else
		{
			BackgroundBorder->SetBrushColor(White->Default - FLinearColor(0.0f,0.0f,0.0f,0.7f));
		}
	}

	if (IsCraftable())
	{
		RecipeIconBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		RecipeIconBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
	}
}

void URecipeIconWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	Hovering = true;
}

void URecipeIconWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	Hovering = false;
}

void URecipeIconWidget::OnClicked()
{
	ParentMenu->SetSelectedRecipe(ParentEntry.RecipeID);
}