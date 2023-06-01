// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingMenuWidget.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "CraftingRecipeCategoryButton.h"
#include "RecipeIconWidget.h"
#include "IngredientRowWidget.h"
#include "WildOmission/Components/CraftingComponent.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Core/WildOmissionStatics.h"
#include "UObject/ConstructorHelpers.h"

UCraftingMenuWidget::UCraftingMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<URecipeIconWidget> RecipeIconWidgetBlueprint(TEXT("/Game/WildOmission/UI/Crafting/WBP_RecipeIcon"));
	ConstructorHelpers::FClassFinder<UIngredientRowWidget> IngredientRowWidgetBlueprint(TEXT("/Game/WildOmission/UI/Crafting/WBP_IngredientRow"));

	if (RecipeIconWidgetBlueprint.Class == nullptr || IngredientRowWidgetBlueprint.Class == nullptr)
	{
		return;
	}

	RecipeIconWidgetClass = RecipeIconWidgetBlueprint.Class;
	IngredientRowWidgetClass = IngredientRowWidgetBlueprint.Class;
}

void UCraftingMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (OwningPawn == nullptr)
	{
		return;
	}

	UInventoryComponent* OwnerInventory = OwningPawn->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventory == nullptr)
	{
		return;
	}

	OwnerInventory->OnUpdate.AddDynamic(this, &UCraftingMenuWidget::Refresh);

	CategoryFilter = EItemCategory::Item;

	SortByButton_All->Setup(this, EItemCategory::All);
	SortByButton_Items->Setup(this, EItemCategory::Item);
	SortByButton_Resources->Setup(this, EItemCategory::Resource);
	SortByButton_Tools->Setup(this, EItemCategory::Tool);
	SortByButton_Building->Setup(this, EItemCategory::Building);
	SortByButton_Weapons->Setup(this, EItemCategory::Weapon);
	SortByButton_Ammo->Setup(this, EItemCategory::Ammo);
	SortByButton_Medical->Setup(this, EItemCategory::Medical);
	SortByButton_Clothing->Setup(this, EItemCategory::Clothing);

	CraftButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::Craft);

	SelectedRecipe = FName("");
}

void UCraftingMenuWidget::Refresh()
{
	RefreshRecipesList();
	RefreshDetailsPanel();
}

void UCraftingMenuWidget::SetCategoryFilter(TEnumAsByte<EItemCategory> NewCategory)
{
	CategoryFilter = NewCategory;
	RefreshRecipesList();
	UE_LOG(LogTemp, Display, TEXT("Selected new item category: %i"), NewCategory);
}

void UCraftingMenuWidget::SetSelectedRecipe(const FName& SelectedRecipeName)
{
	SelectedRecipe = SelectedRecipeName;
	RefreshDetailsPanel();
}

FName UCraftingMenuWidget::GetSelectedRecipe() const
{
	return SelectedRecipe;
}

void UCraftingMenuWidget::RefreshRecipesList()
{
	TArray<FCraftingRecipeEntry> RecipeEntries;
	for (const FName& RecipeID : UCraftingComponent::GetAllRecipes())
	{
		FItemData* YieldItemData = UWildOmissionStatics::GetItemData(RecipeID);
		if (CategoryFilter != EItemCategory::All && YieldItemData->Category != CategoryFilter)
		{
			continue;
		}

		FCraftingRecipe* RecipeData = UCraftingComponent::GetRecipe(RecipeID);

		FCraftingRecipeEntry Entry;
		Entry.RecipeID = RecipeID;
		Entry.CanCraft = CanCraftRecipe(RecipeID);
		Entry.SortPriority = RecipeData->SortPriority;
		Entry.YieldItemData = UWildOmissionStatics::GetItemData(RecipeID);

		RecipeEntries.Add(Entry);
	}
	
	Algo::Sort(RecipeEntries, RecipeSortCraftablePredicate);
	
	RecipesWrapBox->ClearChildren();
	for (const FCraftingRecipeEntry& RecipeEntry : RecipeEntries)
	{
		URecipeIconWidget* NewRecipeIcon = CreateWidget<URecipeIconWidget>(this, RecipeIconWidgetClass);
		if (NewRecipeIcon == nullptr)
		{
			continue;
		}

		NewRecipeIcon->Setup(this, RecipeEntry.RecipeID, RecipeEntry.YieldItemData->Thumbnail);
		RecipesWrapBox->AddChild(NewRecipeIcon);
	}

}

bool UCraftingMenuWidget::RecipeSortCraftablePredicate(const FCraftingRecipeEntry& EntryA, const FCraftingRecipeEntry& EntryB)
{
	return (!EntryA.CanCraft && EntryB.CanCraft);
}

bool UCraftingMenuWidget::RecipeSortPriorityPredicate(const FCraftingRecipeEntry& EntryA, const FCraftingRecipeEntry& EntryB)
{
	return EntryA.SortPriority < EntryB.SortPriority && !RecipeSortCraftablePredicate(EntryA, EntryB);
}

bool UCraftingMenuWidget::RecipeSortNamePredicate(const FCraftingRecipeEntry& EntryA, const FCraftingRecipeEntry& EntryB)
{
	return EntryA.RecipeID.Compare(EntryB.RecipeID) > 0 && !RecipeSortPriorityPredicate(EntryA, EntryB);
}

void UCraftingMenuWidget::RefreshDetailsPanel()
{
	if (SelectedRecipe == FName())
	{
		ClearDetailsPanel();
		return;
	}

	FCraftingRecipe* RecipeData = UCraftingComponent::GetRecipe(SelectedRecipe);
	if (RecipeData == nullptr)
	{
		return;
	}

	FItemData* RecipeYieldItemData = UWildOmissionStatics::GetItemData(SelectedRecipe);
	if (RecipeYieldItemData == nullptr)
	{
		return;
	}

	SelectedRecipeNameTextBlock->SetText(FText::FromString(RecipeYieldItemData->DisplayName));
	
	SelectedRecipeDescriptionTextBlock->SetText(FText::FromString(RecipeYieldItemData->Description));

	SelectedRecipeIconImage->SetBrushFromMaterial(RecipeYieldItemData->Thumbnail);
	SelectedRecipeIconImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	
	FString RecipeYieldQuantityString;

	if (RecipeData->YieldQuantity > 1)
	{
		RecipeYieldQuantityString = FString::Printf(TEXT("x%i"), RecipeData->YieldQuantity);
	}

	SelectedRecipeYieldTextBlock->SetText(FText::FromString(RecipeYieldQuantityString));

	RefreshIngredientList();

	CraftButton->SetIsEnabled(CanCraftRecipe(SelectedRecipe));
}

void UCraftingMenuWidget::ClearDetailsPanel()
{
	SelectedRecipeNameTextBlock->SetText(FText::FromString(FString()));
	
	SelectedRecipeDescriptionTextBlock->SetText(FText::FromString(FString()));

	SelectedRecipeIconImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));

	CraftButton->SetIsEnabled(false);

	IngredientListBox->ClearChildren();
}

void UCraftingMenuWidget::RefreshIngredientList()
{
	IngredientListBox->ClearChildren();
	
	FCraftingRecipe* RecipeData = UCraftingComponent::GetRecipe(SelectedRecipe);
	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	if (RecipeData == nullptr || OwnerInventoryComponent == nullptr)
	{
		return;
	}

	for (const FInventoryItem& Ingredient : RecipeData->Ingredients)
	{
		int32 HasAmount = OwnerInventoryComponent->GetContents()->GetItemQuantity(Ingredient.Name);
		FItemData* IngredientItemData = UWildOmissionStatics::GetItemData(Ingredient.Name);
		if (IngredientItemData == nullptr)
		{
			return;
		}

		UIngredientRowWidget* NewIngredientRow = CreateWidget<UIngredientRowWidget>(this, IngredientRowWidgetClass);
		if (NewIngredientRow == nullptr)
		{
			return;
		}
		NewIngredientRow->Setup(IngredientItemData->DisplayName, Ingredient.Quantity, HasAmount);
		IngredientListBox->AddChild(NewIngredientRow);
		
		UVerticalBoxSlot* IngredientRowSlot = Cast<UVerticalBoxSlot>(NewIngredientRow->Slot);
		if (IngredientRowSlot == nullptr)
		{
			return;
		}
		FMargin RowPadding;
		RowPadding.Top = 5.0f;
		RowPadding.Bottom = 5.0f;
		RowPadding.Left = 5.0f;
		RowPadding.Right = 5.0f;

		IngredientRowSlot->SetPadding(RowPadding);
	}
}

void UCraftingMenuWidget::Craft()
{
	UCraftingComponent* OwnerCraftingComponent = GetOwningPlayerPawn()->FindComponentByClass<UCraftingComponent>();
	if (OwnerCraftingComponent == nullptr)
	{
		return;
	}
	
	OwnerCraftingComponent->Server_CraftItem(SelectedRecipe);
}

bool UCraftingMenuWidget::CanCraftRecipe(const FName& RecipeName)
{
	FCraftingRecipe* RecipeData = UCraftingComponent::GetRecipe(RecipeName);
	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	if (RecipeData == nullptr || OwnerInventoryComponent == nullptr)
	{
		return false;
	}

	for (const FInventoryItem& Ingredient : RecipeData->Ingredients)
	{
		int32 IngredientHasAmount = OwnerInventoryComponent->GetContents()->GetItemQuantity(Ingredient.Name);
		if (IngredientHasAmount < Ingredient.Quantity)
		{
			return false;
		}
	}

	return true;
}