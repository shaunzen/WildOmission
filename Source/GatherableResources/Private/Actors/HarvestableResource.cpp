// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/HarvestableResource.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ChunkManager.h"
#include "Components/ChunkInvokerComponent.h"
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

bool AHarvestableResource::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	return AChunkManager::IsActorNetRelevent(this, ViewTarget);
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
	HarvestingInventoryComponent->AddItem(Drop, true);

	// Calculate Rare Drops
	const bool GiveRareDrop = !RareDrops.IsEmpty() && UKismetMathLibrary::RandomBoolWithWeight(NormalizedRareDropChance);
	if (GiveRareDrop)
	{
		FInventoryItem RareDrop = HandleYieldFromList(RareDrops, GatherMultiplier);
		HarvestingInventoryComponent->AddItem(RareDrop, true);
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

void AHarvestableResource::BeginPlay()
{
	Super::BeginPlay();

	AChunkManager::HandleActorRenderDistanceVisibility(this);

	// Get Chunk Manager
	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	// Get Surface Type at location
	const uint8 SurfaceType = ChunkManager->GetSurfaceTypeAtLocation(this->GetActorLocation());

	// Set property acordingly
	MeshComponent->SetDefaultCustomPrimitiveDataFloat(0, static_cast<float>(SurfaceType == 6));
}
