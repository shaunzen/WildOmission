// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/WorldGenerationHandler.h"
#include "Actors/SaveHandler.h"
#include "Interfaces/GameSaveLoadController.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

#include "DrawDebugHelpers.h"

static UDataTable* BiomeGenerationDataTable = nullptr;

// Sets default values
AWorldGenerationHandler::AWorldGenerationHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> BiomeDataTableBlueprint(TEXT("/Game/WorldGeneration/DataTables/DT_BiomeGenerationData"));
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

	RegenerationTimerDelegate.BindUObject(this, &AWorldGenerationHandler::CheckNodeRegenerationConditions);
	GetWorld()->GetTimerManager().SetTimer(RegenerationTimerHandle, RegenerationTimerDelegate, 10.0f, true);
}

void AWorldGenerationHandler::Generate(const FWorldGenerationSettings& GenerationSettings)
{
	GenerateTrees(GenerationSettings);
	//GenerateNodes(GenerationSettings);
	GenerateCollectables(GenerationSettings);
	GenerateLootables(GenerationSettings);

	if (SaveHandler == nullptr || SaveHandler->GetSaveLoadController() == nullptr)
	{
		return;
	}

	SaveHandler->GetSaveLoadController()->StopLoading();
}

void AWorldGenerationHandler::CheckNodeRegenerationConditions()
{
	UE_LOG(LogWorldGeneration, Display, TEXT("Checking Node Regeneration Conditions."));
	FWorldGenerationSettings GenerationSettings;

	TArray<AActor*> AllNodesInWorld;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Node"), AllNodesInWorld);
	
	// We only want to do this around players
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			continue;
		}

		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn == nullptr)
		{
			continue;
		}

		DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 5000.0f, 16, FColor::Green, false, 10.0f, 0, 100.0f);
		DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 10000.0f, 16, FColor::Red, false, 10.0f, 0, 100.0f);

		const float MaxRange = 10000.0f;
		TArray<AActor*> NodesWithinRange = FilterActorsByRange(AllNodesInWorld, Pawn->GetActorLocation(), MaxRange);
		if (NodesWithinRange.Num() > GenerationSettings.MinNodeCount)
		{
			UE_LOG(LogWorldGeneration, Display, TEXT("Found %i Nodes Around Player, Less than %i is required to invoke regeneration."), NodesWithinRange.Num(), GenerationSettings.MinNodeCount);
			continue;
		}

		RegenerateNodesAroundOrigin(GenerationSettings, Pawn->GetActorLocation());
	}
}

void AWorldGenerationHandler::RegenerateNodesAroundOrigin(const FWorldGenerationSettings& GenerationSettings, const FVector& Origin)
{
	const int32 AmountOfNodesToSpawn = FMath::RandRange(1, 10);
	
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(TEXT("Plains"));

	for (int32 i = 0; i < AmountOfNodesToSpawn; i++)
	{
		const int32 NodeIndexToSpawn = FMath::RandRange(0, BiomeData->Nodes.Num() - 1);
		FTransform SpawnTransform;
		if (!FindSpawnTransformAroundOrigin(GenerationSettings, Origin, SpawnTransform, true))
		{
			continue;
		}
		GetWorld()->SpawnActor<AActor>(BiomeData->Nodes[NodeIndexToSpawn].BlueprintClass, SpawnTransform);
		UE_LOG(LogWorldGeneration, Display, TEXT("Spawned Node Index: %i, At Location: %s"), NodeIndexToSpawn, *SpawnTransform.GetLocation().ToString());
	}
}

TArray<AActor*> AWorldGenerationHandler::FilterActorsByRange(const TArray<AActor*>& InList, const FVector& Origin, float Range)
{
	TArray<AActor*> FilteredList;
	for (AActor* InActor : InList)
	{
		if (InActor == nullptr)
		{
			continue;
		}

		const float Distance = FVector::Distance(InActor->GetActorLocation(), Origin);
		if (Distance > Range)
		{
			continue;
		}

		FilteredList.Add(InActor);
	}

	return FilteredList;
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
			FTransform SpawnTransform;
			if (!FindSpawnTransform(GenerationSettings, SpawnTransform, Tree.FollowSurfaceNormal))
			{
				continue;
			}

			AActor* SpawnedTree = GetWorld()->SpawnActor<AActor>(Tree.BlueprintClass, SpawnTransform);
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
			FTransform SpawnTransform;
			if (!FindSpawnTransform(GenerationSettings, SpawnTransform, Node.FollowSurfaceNormal))
			{
				continue;
			}

			AActor* SpawnedNode = GetWorld()->SpawnActor<AActor>(Node.BlueprintClass, SpawnTransform);
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
			FTransform SpawnTransform;
			if (!FindSpawnTransform(GenerationSettings, SpawnTransform, Collectable.FollowSurfaceNormal))
			{
				continue;
			}

			AActor* SpawnedCollectable = GetWorld()->SpawnActor<AActor>(Collectable.BlueprintClass, SpawnTransform);
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
			FTransform SpawnTransform;
			if (!FindSpawnTransform(GenerationSettings, SpawnTransform, Lootable.FollowSurfaceNormal))
			{
				continue;
			}

			AActor* SpawnedLootCrate = GetWorld()->SpawnActor<AActor>(Lootable.BlueprintClass, SpawnTransform);
		}
	}
}

bool AWorldGenerationHandler::FindSpawnLocation(const FWorldGenerationSettings& GenerationSettings, FVector& OutLocation, FVector& OutSurfaceNormal)
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
		OutSurfaceNormal = HitResult.ImpactNormal;
		return true;
	}
	return false;
}

bool AWorldGenerationHandler::FindSpawnTransform(const FWorldGenerationSettings& GenerationSettings, FTransform& OutTransform, bool FollowSurfaceNormal)
{
	int32 HalfWorldCentimetersX = (GenerationSettings.WorldSizeMetersX * 0.5f) * 100;
	int32 HalfWorldCentimetersY = (GenerationSettings.WorldSizeMetersY * 0.5f) * 100;

	FHitResult HitResult;
	FVector Start = FVector(FMath::RandRange(-HalfWorldCentimetersX, HalfWorldCentimetersX), FMath::RandRange(-HalfWorldCentimetersY, HalfWorldCentimetersY), GenerationSettings.WorldHeightMeters * 100);
	FVector End = Start - FVector(0, 0, GenerationSettings.WorldHeightMeters * 200);

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
	{
		return false;
	}

	// SurfaceType1 = Grass
	if (HitResult.PhysMaterial == nullptr || HitResult.PhysMaterial->SurfaceType != SurfaceType1)
	{
		return false;
	}

	OutTransform.SetLocation(HitResult.ImpactPoint);

	// Find the Rotation
	FRotator SpawnRotation = FRotator::ZeroRotator;
	float YawOffset = FMath::RandRange(0.0f, 360.0f);
	if (FollowSurfaceNormal)
	{
		SpawnRotation = HitResult.ImpactNormal.RotateAngleAxis(90.0f, FVector(0.0f, 1.0f, 0.0f)).Rotation();
	}

	SpawnRotation.Yaw += YawOffset;
	OutTransform.SetRotation(FQuat(SpawnRotation));

	return true;
}

bool AWorldGenerationHandler::FindSpawnTransformAroundOrigin(const FWorldGenerationSettings& GenerationSettings, const FVector& Origin, FTransform& OutTransform, bool FollowSurfaceNormal)
{
	const float InnerSpawnRadiusCentimeters = 5000.0f;
	const float OuterSpawnRadiusCentimeters = 10000.0f;

	const float TraceHeight = 50000.0f;
	const float SpawnDistance = FMath::RandRange(InnerSpawnRadiusCentimeters, OuterSpawnRadiusCentimeters);
	const float SpawnAngle = FMath::RandRange(0.0f, 360.0f);

	FVector SpawnLocationWithinRadius = FVector::ForwardVector * SpawnDistance;
	SpawnLocationWithinRadius = SpawnLocationWithinRadius.RotateAngleAxis(SpawnAngle, FVector::UpVector);

	FVector Start = SpawnLocationWithinRadius + Origin;
	Start.Z = TraceHeight;
	FVector End = Start - FVector(0.0f, 0.0f, TraceHeight * 2.0f);

	FHitResult HitResult;
	
	//FVector Start = FVector(FMath::RandRange(-HalfWorldCentimetersX, HalfWorldCentimetersX), FMath::RandRange(-HalfWorldCentimetersY, HalfWorldCentimetersY), GenerationSettings.WorldHeightMeters * 100);
	//FVector End = Start - FVector(0, 0, GenerationSettings.WorldHeightMeters * 200);

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
	{
		return false;
	}

	// SurfaceType1 = Grass
	if (HitResult.PhysMaterial == nullptr || HitResult.PhysMaterial->SurfaceType != SurfaceType1)
	{
		return false;
	}

	OutTransform.SetLocation(HitResult.ImpactPoint);

	// Find the Rotation
	FRotator SpawnRotation = FRotator::ZeroRotator;
	float YawOffset = FMath::RandRange(0.0f, 360.0f);
	if (FollowSurfaceNormal)
	{
		FRotator ImpactRotation = HitResult.ImpactNormal.RotateAngleAxis(90.0f, FVector(0.0f, 1.0f, 0.0f)).Rotation();
		SpawnRotation.Pitch = ImpactRotation.Pitch;
		SpawnRotation.Roll = ImpactRotation.Roll;
	}

	SpawnRotation.Yaw = YawOffset;
	OutTransform.SetRotation(FQuat(SpawnRotation));

	return true;
}
