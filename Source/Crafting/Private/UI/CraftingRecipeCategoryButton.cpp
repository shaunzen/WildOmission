// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingRecipeCategoryButton.h"
#include "UI/CraftingMenuWidget.h"
#include "Structs/ItemData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UIHelpers.h"

UCraftingRecipeCategoryButton::UCraftingRecipeCategoryButton(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ParentCraftingMenu = nullptr;
	Category = EItemCategory::Item;
}

void UCraftingRecipeCategoryButton::Setup(UCraftingMenuWidget* InParent, TEnumAsByte<EItemCategory> InCategory)
{
	Button->OnClicked.AddDynamic(this, &UCraftingRecipeCategoryButton::OnClicked);
	
	FString ButtonTextString = FString();
	switch (InCategory)
	{
	case All:
		ButtonTextString = FString("All");
		break;
	case Item:
		ButtonTextString = FString("Items");
		break;
	case Resource:
		ButtonTextString = FString("Resource");
		break;
	case Tool:
		ButtonTextString = FString("Tools");
		break;
	case Building:
		ButtonTextString = FString("Building");
		break;
	case Weapon:
		ButtonTextString = FString("Weapons");
		break;
	case Ammo:
		ButtonTextString = FString("Ammo");
		break;
	case Medical:
		ButtonTextString = FString("Medical");
		break;
	case Clothing:
		ButtonTextString = FString("Clothing");
		break;
	default:
		ButtonTextString = FString("Default");
		break;
	}
	
	TextBlock->SetText(FText::FromString(ButtonTextString));

	ParentCraftingMenu = InParent;
	Category = InCategory;
}

void UCraftingRecipeCategoryButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ParentCraftingMenu && ParentCraftingMenu->GetCategoryFilter() == Category)
	{
		FUIColor* Blue = UUIHelpers::GetUIColor(FName("Blue"));
		Button->SetBackgroundColor(Blue->Default);
	}
	else
	{
		FUIColor* DarkGray = UUIHelpers::GetUIColor(FName("DarkGray"));
		Button->SetBackgroundColor(DarkGray->Default);
	}
}

void UCraftingRecipeCategoryButton::OnClicked()
{
	if (ParentCraftingMenu == nullptr)
	{
		return;
	}

	ParentCraftingMenu->SetCategoryFilter(Category);
}