// Copyright Telephone Studios. All Rights Reserved.


#include "WorldGenerationHandlerComponent.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "SaveHandler.h"
#include "WildOmission/Resources/HarvestableResource.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

static UDataTable* BiomeGenerationDataTable = nullptr;

// Sets default values for this component's properties
UWorldGenerationHandlerComponent::UWorldGenerationHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> BiomeDataTableBlueprint(TEXT("/Game/WildOmission/Core/DataTables/DT_BiomeGenerationData"));
	if (BiomeDataTableBlueprint.Succeeded())
	{
		BiomeGenerationDataTable = BiomeDataTableBlueprint.Object;
	}
}

void UWorldGenerationHandlerComponent::Generate(const FWorldGenerationSettings& GenerationSettings)
{
	GenerateTrees(GenerationSettings);
	GenerateNodes(GenerationSettings);
	GenerateCollectables(GenerationSettings);
	GenerateLootables(GenerationSettings);
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->StopLoading();
}

void UWorldGenerationHandlerComponent::RegenerateResources(const FWorldGenerationSettings& GenerationSettings)
{
	FWorldGenerationSettings RegenerationSettings = GenerationSettings;
	RegenerationSettings.SpawnLimiter = 0.1f;
	
	GenerateNodes(RegenerationSettings);
	GenerateCollectables(RegenerationSettings);
	GenerateLootables(RegenerationSettings);
}

FVector2D UWorldGenerationHandlerComponent::GetWorldSizeMeters()
{
	FWorldGenerationSettings Settings;
	return FVector2D(Settings.WorldSizeMetersX, Settings.WorldSizeMetersY);
}

FBiomeGenerationData* UWorldGenerationHandlerComponent::GetBiomeGenerationData(const FName& BiomeName)
{
	if (BiomeGenerationDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Biome Generation Data Context"));

	return BiomeGenerationDataTable->FindRow<FBiomeGenerationData>(BiomeName, ContextString, true);
}

void UWorldGenerationHandlerComponent::GenerateTrees(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FHarvestableResourceData& Tree : BiomeData->Trees)
	{
		int32 AmountOfTreeToSpawn = FMath::RoundToInt32((WorldAreaMeters * (Tree.DensityPerMeter * GenerationSettings.SpawnLimiter)) / BiomeData->Trees.Num());
		for (int32 i = 0; i < AmountOfTreeToSpawn; i++)
		{
			FVector LocationToSpawn = FVector::ZeroVector;
			FRotator RotationToSpawn = FRotator::ZeroRotator;
			RotationToSpawn.Yaw = FMath::RandRange(0, 360);

			if (!FindSpawnLocation(GenerationSettings, LocationToSpawn))
			{
				continue;
			}

			AHarvestableResource* SpawnedTree = GetWorld()->SpawnActor<AHarvestableResource>(Tree.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
	
}

void UWorldGenerationHandlerComponent::GenerateNodes(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FHarvestableResourceData& Node : BiomeData->Nodes)
	{
		int32 AmountOfNodeToSpawn = FMath::RoundToInt32((WorldAreaMeters * (Node.DensityPerMeter * GenerationSettings.SpawnLimiter)) / BiomeData->Nodes.Num());
		for (int32 i = 0; i < AmountOfNodeToSpawn; i++)
		{
			FVector LocationToSpawn = FVector::ZeroVector;
			FRotator RotationToSpawn = FRotator::ZeroRotator;
			RotationToSpawn.Yaw = FMath::RandRange(0, 360);

			if (!FindSpawnLocation(GenerationSettings, LocationToSpawn))
			{
				continue;
			}

			AHarvestableResource* SpawnedNode = GetWorld()->SpawnActor<AHarvestableResource>(Node.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
}

void UWorldGenerationHandlerComponent::GenerateCollectables(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FCollectableResourceData& Collectable : BiomeData->Collectables)
	{
		int32 AmountOfCollectableToSpawn = FMath::RoundToInt32((WorldAreaMeters * (Collectable.DensityPerMeter * GenerationSettings.SpawnLimiter)) / BiomeData->Collectables.Num());
		for (int32 i = 0; i < AmountOfCollectableToSpawn; i++)
		{
			FVector LocationToSpawn = FVector::ZeroVector;
			FRotator RotationToSpawn = FRotator::ZeroRotator;
			RotationToSpawn.Yaw = FMath::RandRange(0, 360);
			if (!FindSpawnLocation(GenerationSettings, LocationToSpawn))
			{
				continue;
			}

			ACollectableResource* SpawnedCollectable = GetWorld()->SpawnActor<ACollectableResource>(Collectable.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
}

void UWorldGenerationHandlerComponent::GenerateLootables(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FLootableData& Lootable: BiomeData->Lootables)
	{
		int32 AmountOfLootableToSpawn = FMath::RoundToInt32((WorldAreaMeters * (Lootable.DensityPerMeter * GenerationSettings.SpawnLimiter)) / BiomeData->Lootables.Num());
		for (int32 i = 0; i < AmountOfLootableToSpawn; i++)
		{
			FVector LocationToSpawn = FVector::ZeroVector;
			FRotator RotationToSpawn = FRotator::ZeroRotator;
			RotationToSpawn.Yaw = FMath::RandRange(0, 360);
			if (!FindSpawnLocation(GenerationSettings, LocationToSpawn))
			{
				continue;
			}

			ALootCrateBase* SpawnedLootCrate = GetWorld()->SpawnActor<ALootCrateBase>(Lootable.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
}

bool UWorldGenerationHandlerComponent::FindSpawnLocation(const FWorldGenerationSettings& GenerationSettings, FVector& OutLocation)
{
	int32 HalfWorldCentimetersX = (GenerationSettings.WorldSizeMetersX * 0.5f) * 100;
	int32 HalfWorldCentimetersY = (GenerationSettings.WorldSizeMetersY * 0.5f) * 100;

	FHitResult HitResult;
	FVector Start = FVector(FMath::RandRange(-HalfWorldCentimetersX, HalfWorldCentimetersX), FMath::RandRange(-HalfWorldCentimetersY, HalfWorldCentimetersY), GenerationSettings.WorldHeightMeters * 100);
	FVector End = Start - FVector(0, 0, GenerationSettings.WorldHeightMeters * 200);

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
	{
																					// SurfaceType1 = Grass
		if (HitResult.PhysMaterial == nullptr || HitResult.PhysMaterial->SurfaceType != SurfaceType1)
		{
			return false;
		}

		OutLocation = HitResult.ImpactPoint;
		return true;
	}
	return false;
}