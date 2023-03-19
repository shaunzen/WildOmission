// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingWidget.h"
#include "Components/Button.h"
#include "WildOmission/Components/CraftingComponent.h"

bool UCraftingWidget::Initialize()
{
	bool Success = Super::Initialize();
	
	if (Success == false
		|| CraftPickaxeButton == nullptr
		|| CraftHatchetButton == nullptr)
	{
		return false;
	}

	CraftPickaxeButton->OnClicked.AddDynamic(this, &UCraftingWidget::PickaxeButtonClicked);
	CraftHatchetButton->OnClicked.AddDynamic(this, &UCraftingWidget::HatchetButtonClicked);

	return true;
}

void UCraftingWidget::PickaxeButtonClicked()
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
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Crafting Pickaxe"));
}

void UCraftingWidget::HatchetButtonClicked()
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
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Crafting Hatchet"));
}