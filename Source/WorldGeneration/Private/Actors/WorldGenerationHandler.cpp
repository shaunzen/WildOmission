// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/WorldGenerationHandler.h"
#include "Actors/SaveHandler.h"
#include "Interfaces/GameSaveLoadController.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

static UDataTable* BiomeGenerationDataTable = nullptr;

// Sets default values
AWorldGenerationHandler::AWorldGenerationHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> BiomeDataTableBlueprint(TEXT("/Game/WildOmission/Core/DataTables/DT_BiomeGenerationData"));
	if (BiomeDataTableBlueprint.Succeeded())
	{
		BiomeGenerationDataTable = BiomeDataTableBlueprint.Object;
	}
}

void AWorldGenerationHandler::GenerateLevel(ASaveHandler* InstigatingSaveHandler, UWildOmissionSaveGame* InSaveFile)
{
	FWorldGenerationSettings GenerationSettings;
	FTimerHandle WorldGenerationTimerHandle;
	FTimerDelegate WorldGenerationTimerDelegate;

	WorldGenerationTimerDelegate.BindUFunction(this, FName("Generate"), GenerationSettings, InSaveFile);

	GetWorld()->GetTimerManager().SetTimer(WorldGenerationTimerHandle, WorldGenerationTimerDelegate, 1.0f, false);

	InSaveFile->CreationInformation.LevelHasGenerated = true;
}

// Called when the game starts or when spawned
void AWorldGenerationHandler::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle RegenerationTimerHandle;
	FTimerDelegate RegenerationTimerDelegate;

	RegenerationTimerDelegate.BindUObject(this, &AWorldGenerationHandler::CheckRegenerationConditions);
	GetWorld()->GetTimerManager().SetTimer(RegenerationTimerHandle, RegenerationTimerDelegate, 120, true);
}

void AWorldGenerationHandler::Generate(const FWorldGenerationSettings& GenerationSettings)
{
	GenerateTrees(GenerationSettings);
	GenerateNodes(GenerationSettings);
	GenerateCollectables(GenerationSettings);
	GenerateLootables(GenerationSettings);

	if (SaveHandler == nullptr || SaveHandler->GetSaveLoadController() == nullptr)
	{
		return;
	}

	SaveHandler->GetSaveLoadController()->StopLoading();
}

void AWorldGenerationHandler::CheckRegenerationConditions()
{
	FWorldGenerationSettings GenerationSettings;

	TArray<AActor*> AllNodesInWorld;
	TArray<AActor*> AllCollectablesInWorld;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Node"), AllNodesInWorld);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Collectable"), AllCollectablesInWorld);

	if (AllNodesInWorld.Num() > GenerationSettings.MinNodeCount || AllCollectablesInWorld.Num() > GenerationSettings.MinCollectableCount)
	{
		return;
	}

	RegenerateResources(GenerationSettings);
}

void AWorldGenerationHandler::RegenerateResources(const FWorldGenerationSettings& GenerationSettings)
{
	FWorldGenerationSettings RegenerationSettings = GenerationSettings;
	RegenerationSettings.SpawnLimiter = 0.1f;

	GenerateNodes(RegenerationSettings);
	GenerateCollectables(RegenerationSettings);
	GenerateLootables(RegenerationSettings);
}

FVector2D AWorldGenerationHandler::GetWorldSizeMeters()
{
	FWorldGenerationSettings Settings;
	return FVector2D(Settings.WorldSizeMetersX, Settings.WorldSizeMetersY);
}

FBiomeGenerationData* AWorldGenerationHandler::GetBiomeGenerationData(const FName& BiomeName)
{
	if (BiomeGenerationDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Biome Generation Data Context"));

	return BiomeGenerationDataTable->FindRow<FBiomeGenerationData>(BiomeName, ContextString, true);
}

void AWorldGenerationHandler::GenerateTrees(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FSpawnData& Tree : BiomeData->Trees)
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

			AActor* SpawnedTree = GetWorld()->SpawnActor<AActor>(Tree.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
}

void AWorldGenerationHandler::GenerateNodes(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FSpawnData& Node : BiomeData->Nodes)
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

			AActor* SpawnedNode = GetWorld()->SpawnActor<AActor>(Node.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
}

void AWorldGenerationHandler::GenerateCollectables(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FSpawnData& Collectable : BiomeData->Collectables)
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

			AActor* SpawnedCollectable = GetWorld()->SpawnActor<AActor>(Collectable.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
}

void AWorldGenerationHandler::GenerateLootables(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FSpawnData& Lootable : BiomeData->Lootables)
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

			AActor* SpawnedLootCrate = GetWorld()->SpawnActor<AActor>(Lootable.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
}

bool AWorldGenerationHandler::FindSpawnLocation(const FWorldGenerationSettings& GenerationSettings, FVector& OutLocation)
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