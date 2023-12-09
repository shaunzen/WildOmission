// Copyright Telephone Studios. All Rights Reserved.


#include "WorldGenerationHandler.h"
#include "Components/PreventExtinctionComponent.h"
#include "Components/ResourceRegenerationComponent.h"
#include "Chunk.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static AWorldGenerationHandler* Instance = nullptr;
static UDataTable* BiomeGenerationDataTable = nullptr;

// Sets default values
AWorldGenerationHandler::AWorldGenerationHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PreventExtinctionComponent = CreateDefaultSubobject<UPreventExtinctionComponent>(TEXT("PreventExtinctionComponent"));
	RegenerationComponent = CreateDefaultSubobject<UResourceRegenerationComponent>(TEXT("RegenerationComponent"));

	static ConstructorHelpers::FObjectFinder<UDataTable> BiomeDataTableBlueprint(TEXT("/Game/WorldGeneration/DataTables/DT_BiomeGenerationData"));
	if (BiomeDataTableBlueprint.Succeeded())
	{
		BiomeGenerationDataTable = BiomeDataTableBlueprint.Object;
	}

	static ConstructorHelpers::FClassFinder<AChunk> ChunkBlueprint(TEXT("/Game/WorldGeneration/BP_Chunk"));
	if (ChunkBlueprint.Succeeded())
	{
		ChunkClass = ChunkBlueprint.Class;
	}
}

void AWorldGenerationHandler::GenerateLevel()
{
	FWorldGenerationSettings GenerationSettings;
	FTimerHandle WorldGenerationTimerHandle;
	FTimerDelegate WorldGenerationTimerDelegate;
	WorldGenerationTimerDelegate.BindUFunction(this, TEXT("Generate"), GenerationSettings);
	GetWorld()->GetTimerManager().SetTimer(WorldGenerationTimerHandle, WorldGenerationTimerDelegate, 1.0f, false);
}

// Called when the game starts or when spawned
void AWorldGenerationHandler::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr || World->IsEditorWorld() && IsValid(Instance))
	{
		return;
	}

	Instance = this;
}

void AWorldGenerationHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Instance = nullptr;
}

void AWorldGenerationHandler::Generate(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	//GenerateResource(BiomeData->Trees, GenerationSettings, false);
	//GenerateResource(BiomeData->Nodes, GenerationSettings, true);
	//GenerateResource(BiomeData->Collectables, GenerationSettings, true);
	//GenerateResource(BiomeData->Lootables, GenerationSettings, true);
	
	// TODO generate chunks

	GenerateChunks();

	if (OnGenerationComplete.IsBound())
	{
		OnGenerationComplete.Broadcast();
	}
}

void AWorldGenerationHandler::GenerateChunks()
{
	const int32 WorldSize = 10;
	for (int32 X = 0; X < WorldSize; ++X)
	{
		for (int32 Y = 0; Y < WorldSize; ++Y)
		{
			// generate chunk
			const FVector ChunkLocation(X * 1600.0f, Y * 1600.0f, 0.0f);
			AChunk* SpawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunkClass, ChunkLocation, FRotator::ZeroRotator);

			// TODO add to chunk list for keeping track of
		}
	}
}

void AWorldGenerationHandler::PreventExtinction()
{
	if (PreventExtinctionComponent == nullptr)
	{
		return;
	}

	PreventExtinctionComponent->PreventExtinction();
}

FVector2D AWorldGenerationHandler::GetWorldSizeMeters()
{
	FWorldGenerationSettings Settings;
	return FVector2D(Settings.WorldSizeMetersX, Settings.WorldSizeMetersY);
}

TArray<FBiomeGenerationData*> AWorldGenerationHandler::GetAllPossibleBiomes()
{
	TArray<FBiomeGenerationData*> Biomes;
	if (BiomeGenerationDataTable == nullptr)
	{
		return Biomes;
	}

	static const FString ContextString(TEXT("All Biomes Data Context"));
	
	BiomeGenerationDataTable->GetAllRows<FBiomeGenerationData>(ContextString, Biomes);

	return Biomes;
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

AWorldGenerationHandler* AWorldGenerationHandler::GetWorldGenerationHandler()
{
	return Instance;
}

void AWorldGenerationHandler::GenerateResource(const TArray<FSpawnData>& SpawnData, const FWorldGenerationSettings& GenerationSettings, bool FollowSurfaceNormal)
{
	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FSpawnData& Resource : SpawnData)
	{
		int32 AmountOfResourceToSpawn = FMath::RoundToInt32((WorldAreaMeters * (Resource.DensityPerMeter * GenerationSettings.SpawnLimiter)) / SpawnData.Num());
		for (int32 i = 0; i < AmountOfResourceToSpawn; i++)
		{
			FTransform SpawnTransform;
			if (!FindSpawnTransformRandomLocation(GetWorld(), SpawnTransform, GenerationSettings, FollowSurfaceNormal))
			{
				continue;
			}

			AActor* SpawnedResource = GetWorld()->SpawnActor<AActor>(Resource.BlueprintClass, SpawnTransform);
		}
	}
}

bool AWorldGenerationHandler::FindSpawnTransformRandomLocation(UWorld* WorldContextObject, FTransform& OutTransform, const FWorldGenerationSettings& GenerationSettings, bool FollowSurfaceNormal)
{
	if (WorldContextObject == nullptr)
	{
		return false;
	}

	int32 HalfWorldCentimetersX = (GenerationSettings.WorldSizeMetersX * 0.5f) * 100;
	int32 HalfWorldCentimetersY = (GenerationSettings.WorldSizeMetersY * 0.5f) * 100;

	FHitResult HitResult;
	FVector Start = FVector(FMath::RandRange(-HalfWorldCentimetersX, HalfWorldCentimetersX),
		FMath::RandRange(-HalfWorldCentimetersY, HalfWorldCentimetersY),
		GenerationSettings.WorldHeightMeters * 100);
	FVector End = Start - FVector(0, 0, GenerationSettings.WorldHeightMeters * 200);

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	if (!WorldContextObject->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
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

bool AWorldGenerationHandler::FindSpawnTransformRadiusFromOrigin(UWorld* WorldContextObject, FTransform& OutTransform, const FVector& Origin, float InnerRadius, float OuterRadius, const FWorldGenerationSettings& GenerationSettings, bool FollowSurfaceNormal)
{
	if (WorldContextObject == nullptr)
	{
		return false;
	}

	const float SpawnDistance = FMath::RandRange(InnerRadius, OuterRadius);
	const float SpawnAngle = FMath::RandRange(0.0f, 360.0f);

	FVector SpawnLocationWithinRadius = FVector::ForwardVector * SpawnDistance;
	SpawnLocationWithinRadius = SpawnLocationWithinRadius.RotateAngleAxis(SpawnAngle, FVector::UpVector);

	FVector Start = SpawnLocationWithinRadius + Origin;
	Start.Z = GenerationSettings.WorldHeightMeters;
	FVector End = Start - FVector(0.0f, 0.0f, GenerationSettings.WorldHeightMeters * 2.0f);

	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	if (!WorldContextObject->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
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
