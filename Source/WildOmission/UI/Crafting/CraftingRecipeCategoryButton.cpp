// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingRecipeCategoryButton.h"
#include "CraftingMenuWidget.h"
#include "WildOmission/Core/Structs/ItemData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCraftingRecipeCategoryButton::Setup(UCraftingMenuWidget* InParent, TEnumAsByte<EItemCategory> InCategory)
{
	Button->OnClicked.AddDynamic(this, &UCraftingRecipeCategoryButton::OnClicked);
	FString ButtonTextString = FString();

	// TODO text switch

	TextBlock->SetText(FText::FromString(ButtonTextString));

	ParentCraftingMenu= InParent;
	Category = InCategory;
}

void UCraftingRecipeCategoryButton::OnClicked()
{
	if (ParentCraftingMenu == nullptr)
	{
		return;
	}
	ParentCraftingMenu->SetCategoryFilter(Category);
}