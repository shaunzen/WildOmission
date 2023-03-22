// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingMenuWidget.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "RecipeIconWidget.h"
#include "IngredientRowWidget.h"
#include "WildOmission/Components/CraftingComponent.h"
#include "WildOmission/Components/InventoryComponent.h"
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

bool UCraftingMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	
	if (Success == false
		|| CraftButton == nullptr)
	{
		return false;
	}

	CraftButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::Craft);
	
	SelectedRecipe = FName("");

	return true;
}

void UCraftingMenuWidget::Refresh()
{
	RefreshRecipesList();
	RefreshDetailsPanel();
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
	// Clear all children of the scroll box
	RecipesWrapBox->ClearChildren();

	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	UCraftingComponent* OwnerCraftingComponent = GetOwningPlayerPawn()->FindComponentByClass<UCraftingComponent>();
	if (OwnerInventoryComponent == nullptr || OwnerCraftingComponent == nullptr)
	{
		return;
	}

	for (const FName& RecipeName : OwnerCraftingComponent->GetAllRecipes())
	{
		FCraftingRecipe* RecipeData = OwnerCraftingComponent->GetRecipe(RecipeName);
		if (RecipeData == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find recipe data for %s"), *RecipeName.ToString());
			return;
		}
		
		FItemData* YeildItemData = OwnerInventoryComponent->GetItemData(RecipeData->Yeild.Name);
		if (YeildItemData == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find item data for recipe yeild %s"), *RecipeData->Yeild.Name.ToString());
			return;
		}

		URecipeIconWidget* NewRecipeEntry = CreateWidget<URecipeIconWidget>(this, RecipeIconWidgetClass);

		NewRecipeEntry->Setup(this, RecipeName, YeildItemData->Thumbnail);
		RecipesWrapBox->AddChild(NewRecipeEntry);
	}
}

void UCraftingMenuWidget::RefreshDetailsPanel()
{
	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	UCraftingComponent* OwnerCraftingComponent = GetOwningPlayerPawn()->FindComponentByClass<UCraftingComponent>();
	if (OwnerInventoryComponent == nullptr || OwnerCraftingComponent == nullptr)
	{
		return;
	}

	if (SelectedRecipe == FName())
	{
		ClearDetailsPanel();
		return;
	}

	FCraftingRecipe* RecipeData = OwnerCraftingComponent->GetRecipe(SelectedRecipe);
	if (RecipeData == nullptr)
	{
		return;
	}

	FItemData* RecipeYeildItemData = OwnerInventoryComponent->GetItemData(RecipeData->Yeild.Name);
	if (RecipeYeildItemData == nullptr)
	{
		return;
	}

	SelectedRecipeNameTextBlock->SetText(FText::FromName(RecipeYeildItemData->DisplayName));
	
	SelectedRecipeDescriptionTextBlock->SetText(FText::FromString(RecipeYeildItemData->Description));

	SelectedRecipeIconImage->SetBrushFromMaterial(RecipeYeildItemData->Thumbnail);
	SelectedRecipeIconImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	RefreshIngredientList(RecipeData, OwnerInventoryComponent);

	CraftButton->SetIsEnabled(CanCraftSelectedRecipe());
}

void UCraftingMenuWidget::ClearDetailsPanel()
{
	SelectedRecipeNameTextBlock->SetText(FText::FromString(FString()));
	SelectedRecipeDescriptionTextBlock->SetText(FText::FromString(FString()));
	SelectedRecipeIconImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));

	//FSlateBrush NewBrush;
	//SelectedRecipeIconImage->SetBrush(NewBrush);

	CraftButton->SetIsEnabled(false);
	IngredientListBox->ClearChildren();
}

void UCraftingMenuWidget::RefreshIngredientList(FCraftingRecipe* RecipeData, UInventoryComponent* OwnerInventoryComponent)
{
	// Clear list
	IngredientListBox->ClearChildren();

	for (const FInventoryItem& Ingredient : RecipeData->Ingredients)
	{
		int32 HasAmount = OwnerInventoryComponent->GetContents()->GetItemQuantity(Ingredient.Name);
		FItemData* IngredientItemData = OwnerInventoryComponent->GetItemData(Ingredient.Name);
		if (IngredientItemData == nullptr)
		{
			return;
		}

		// Create a new ingredient row
		UIngredientRowWidget* NewIngredientRow = CreateWidget<UIngredientRowWidget>(this, IngredientRowWidgetClass);
		if (NewIngredientRow == nullptr)
		{
			return;
		}
		// Populate it with the relevent information
		NewIngredientRow->Setup(IngredientItemData->DisplayName, Ingredient.Quantity, HasAmount);

		// Add to IngredientListBox
		IngredientListBox->AddChild(NewIngredientRow);
		
		// get slot
		UVerticalBoxSlot* IngredientRowSlot = Cast<UVerticalBoxSlot>(NewIngredientRow->Slot);
		if (IngredientRowSlot == nullptr)
		{
			return;
		}

		// set padding
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
	
	OwnerCraftingComponent->Server_CraftItem(SelectedRecipe);
}

bool UCraftingMenuWidget::CanCraftSelectedRecipe()
{
	UInventoryComponent* OwnerInventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	UCraftingComponent* OwnerCraftingComponent = GetOwningPlayerPawn()->FindComponentByClass<UCraftingComponent>();
	if (OwnerInventoryComponent == nullptr || OwnerCraftingComponent == nullptr)
	{
		return false;
	}

	FCraftingRecipe* RecipeData = OwnerCraftingComponent->GetRecipe(SelectedRecipe);

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