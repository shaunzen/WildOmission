// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingMenuWidget.h"
#include "Components/Button.h"
#include "WildOmission/Components/CraftingComponent.h"

bool UCraftingMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	
	if (Success == false
		|| CraftPickaxeButton == nullptr
		|| CraftHatchetButton == nullptr)
	{
		return false;
	}

	CraftPickaxeButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::PickaxeButtonClicked);
	CraftHatchetButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::HatchetButtonClicked);

	return true;
}

void UCraftingMenuWidget::PickaxeButtonClicked()
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
	
	OwnerCraftingComponent->Server_CraftItem(FName("pickaxe"));
}

void UCraftingMenuWidget::HatchetButtonClicked()
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

	OwnerCraftingComponent->Server_CraftItem(FName("hatchet"));
}