// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/HarvestableResource.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Log.h"

// Sets default values
AHarvestableResource::AHarvestableResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 5.0f;
	NetDormancy = ENetDormancy::DORM_DormantAll;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	RootComponent = MeshComponent;

	Durability = 10;
}

void AHarvestableResource::OnHarvest(AActor* HarvestingActor, float GatherMultiplier)
{
	UInventoryComponent* HarvestingInventoryComponent = HarvestingActor->FindComponentByClass<UInventoryComponent>();
	if (HarvestingInventoryComponent == nullptr)
	{
		UE_LOG(LogGatherableResources, Warning, TEXT("Could not harvest resource, HarvestingActor has no InventoryComponent."));
		return;
	}

	HarvestingInventoryComponent->AddItem(HandleYield(GatherMultiplier));

	if (ShouldGiveSpecialItemDrop())
	{
		HarvestingInventoryComponent->AddItem(SpecialItemDrop);
	}

	Durability--;

	if (Durability <= 0)
	{
		Destroy();
	}
}

FInventoryItem AHarvestableResource::HandleYield(float GatherMultiplier)
{
	FInventoryItem ItemToGive;
	ItemToGive = ItemYield;
	ItemToGive.Quantity = ItemYield.Quantity * GatherMultiplier;

	if (ItemToGive.Quantity <= 0)
	{
		ItemToGive.Quantity = 1;
	}

	return ItemToGive;
}

bool AHarvestableResource::ShouldGiveSpecialItemDrop()
{
	if (SpecialItemDrop.Quantity == 0 || SpecialItemDrop.Name == FName())
	{
		return false;
	}

	int32 GiveSpecialItemValue = FMath::RandRange(0, 100);
	if (GiveSpecialItemValue > 85)
	{
		return true;
	}

	return false;
}

TEnumAsByte<EToolType> AHarvestableResource::GetRequiredToolType() const
{
	return RequiredToolType;
}

int32 AHarvestableResource::GetDurability() const
{
	return Durability;
}

void AHarvestableResource::SetDurability(const int32& InDurability)
{
	Durability = InDurability;
}