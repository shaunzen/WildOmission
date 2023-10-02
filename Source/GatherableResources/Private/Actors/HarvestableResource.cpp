// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/HarvestableResource.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavModifierComponent.h"
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

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	RootComponent = MeshComponent;

	Identifier = NAME_None;

	NavigationModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavigationModifier"));

	NormalizedQualityToolDropChance = 0.05f;
	NormalizedQualityToolDropChance = 0.25f;
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
	const int32 DropIndex = FMath::RandRange(0, DropList.Num() - 1);
	if (!DropList.IsEmpty() && DropList.IsValidIndex(DropIndex))
	{
		HarvestingInventoryComponent->AddItem(DropList[DropIndex]);
	}

	// Calculate Rare Drops
	const bool GiveRareDrop = !RareDrops.IsEmpty() && UKismetMathLibrary::RandomBoolWithWeight(NormalizedRareDropChance);
	if (GiveRareDrop)
	{
		const int32 RareDropIndex = FMath::RandRange(0, RareDrops.Num() - 1);
		if (RareDrops.IsValidIndex(RareDropIndex))
		{
			HarvestingInventoryComponent->AddItem(RareDrops[RareDropIndex]);
		}
	}
	
	// TODO should give rare drop, and do this on the component version of this too
	// TODO make LeveledHarvestableResource class that handles giving special drops if using metal tools
	// Or make it a bool and figure out how to integrate it into this class
	Durability--;

	if (Durability <= 0)
	{
		Destroy();
	}
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
