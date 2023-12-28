// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/HarvestableResource.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ChunkManager.h"
#include "NavModifierComponent.h"
#include "Log.h"

// Sets default values
AHarvestableResource::AHarvestableResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = false;
	NetUpdateFrequency = 5.0f;
	NetDormancy = ENetDormancy::DORM_DormantAll;
	NetCullDistanceSquared = AChunkManager::GetRenderDistanceCentimeters() * AChunkManager::GetRenderDistanceCentimeters();

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	RootComponent = MeshComponent;

	Identifier = NAME_None;

	NavigationModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavigationModifier"));

	NormalizedRareDropChance = 0.05f;
	NormalizedQualityToolDropChance = 0.25f;

	RequiredToolType = EToolType::WOOD;

	Durability = 10;
}

void AHarvestableResource::OnHarvest(AActor* HarvestingActor, float GatherMultiplier, bool IsQualityTool)
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
		Destroy();
	}
}

FInventoryItem AHarvestableResource::HandleYieldFromList(const TArray<FInventoryItem>& DropList, float GatherMultiplier)
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

FName AHarvestableResource::GetIdentifier() const
{
	return Identifier;
}