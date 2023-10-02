// Copyright Telephone Studios. All Rights Reserved.


#include "Components/HarvestableComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Log.h"

UHarvestableComponent::UHarvestableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	NormalizedRareDropChance = 0.05f;
	NormalizedQualityToolDropChance = 0.25f;
	
	RequiredToolType = EToolType::WOOD;

	Durability = 5;
}

void UHarvestableComponent::OnHarvest(AActor* HarvestingActor, float GatherMultiplier, bool IsQualityTool)
{
	if (HarvestingActor == nullptr)
	{
		UE_LOG(LogGatherableResources, Warning, TEXT("Could not harvest resource, HarvestingActor was nullptr."));
		return;
	}

	UInventoryComponent* HarvestingInventoryComponent = HarvestingActor->FindComponentByClass<UInventoryComponent>();
	if (HarvestingInventoryComponent == nullptr)
	{
		UE_LOG(LogGatherableResources, Warning, TEXT("Could not harvest resource, HarvestingActor has no InventoryComponent."));
		return;
	}

	// Calculate Normal Drops
	const bool GiveQualityToolDrop = !QualityToolDrops.IsEmpty() && IsQualityTool && UKismetMathLibrary::RandomBoolWithWeight(NormalizedQualityToolDropChance);
	TArray<FInventoryItem>& DropList = GiveQualityToolDrop ? QualityToolDrops : CommonDrops;

	FInventoryItem Drop = HandleYieldFromList(DropList, GatherMultiplier);
	HarvestingInventoryComponent->AddItem(Drop);

	// Calculate Rare Drops
	const bool GiveRareDrop = !RareDrops.IsEmpty() && UKismetMathLibrary::RandomBoolWithWeight(NormalizedRareDropChance);
	if (GiveRareDrop)
	{
		FInventoryItem RareDrop = HandleYieldFromList(RareDrops, GatherMultiplier);
		HarvestingInventoryComponent->AddItem(RareDrop);
	}

	Durability--;

	if (Durability <= 0)
	{
		AActor* OwnerActor = GetOwner();
		if (OwnerActor == nullptr)
		{
			return;
		}
		
		OwnerActor->Destroy();
	}
}

FInventoryItem UHarvestableComponent::HandleYieldFromList(const TArray<FInventoryItem>& DropList, float GatherMultiplier)
{
	FInventoryItem Item;
	const int32 DropIndex = FMath::RandRange(0, DropList.Num() - 1);
	if (!DropList.IsEmpty() && DropList.IsValidIndex(DropIndex))
	{
		Item = DropList[DropIndex];
		Item.Quantity = Item.Quantity * GatherMultiplier;
		if (Item.Quantity < 1)
		{
			Item.Quantity = 1;
		}
	}

	return Item;
}

TEnumAsByte<EToolType> UHarvestableComponent::GetRequiredToolType() const
{
	return RequiredToolType;
}

int32 UHarvestableComponent::GetDurability() const
{
	return Durability;
}

void UHarvestableComponent::SetDurability(const int32& InDurability)
{
	Durability = InDurability;
}