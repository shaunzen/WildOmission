// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingMenuWidget.h"
#include "Components/TileView.h"
#include "WildOmission/Components/CraftingComponent.h"

bool UCraftingMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	
	if (Success == false)
	{
		return false;
	}

	return true;
}

void UCraftingMenuWidget::OnOpen()
{
	// Clear all children of the scroll box
	
	// Get recipe data table entries
	
	// For each entry create a new recipe icon and populate it with the relavent information
	
	
	// needed
	// TSubclassOf pointer to the recipe icon class so we can instanciate them
	// pointer to the scroll box
}

void UCraftingMenuWidget::Craft()
{
	FName RecipeName("pickaxe");

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
	
	OwnerCraftingComponent->Server_CraftItem(RecipeName);
}