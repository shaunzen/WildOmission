// Copyright Telephone Studios. All Rights Reserved.


#include "Components/PreventExtinctionComponent.h"
#include "WorldGenerationHandler.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPreventExtinctionComponent::UPreventExtinctionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPreventExtinctionComponent::BeginPlay()
{
	Super::BeginPlay();

	AWorldGenerationHandler* OwnerWorldGenerationHandler = Cast<AWorldGenerationHandler>(GetOwner());
	if (OwnerWorldGenerationHandler == nullptr)
	{
		return;
	}

	TArray<FBiomeGenerationData*> Biomes = OwnerWorldGenerationHandler->GetAllPossibleBiomes();
	if (Biomes.IsEmpty())
	{
		return;
	}

	for (FBiomeGenerationData* Biome : Biomes)
	{
		RunChecksForBiome(Biome);
	}
}

void UPreventExtinctionComponent::RunChecksForBiome(FBiomeGenerationData* Biome)
{
	if (Biome == nullptr)
	{
		return;
	}

	for (const FSpawnData& Collectable : Biome->Collectables)
	{
		TArray<AActor*> CollectableActorsInWorld;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), Collectable.BlueprintClass, CollectableActorsInWorld);
		if (CollectableActorsInWorld.Num() > 0)
		{
			continue;
		}

		SpawnCollectables(Collectable);
	}
}

void UPreventExtinctionComponent::SpawnCollectables(const FSpawnData& CollectableData)
{
	const int32 SpawnCount = 10;
	FTransform SpawnTransform;
	if (!AWorldGenerationHandler::FindSpawnTransformRandomLocation(GetWorld(), SpawnTransform, FWorldGenerationSettings(), true))
	{
		return;
	}

	GetWorld()->SpawnActor<AActor>(CollectableData.BlueprintClass, SpawnTransform);
}
