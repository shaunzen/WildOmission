// Copyright Telephone Studios. All Rights Reserved.


#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static AChunkManager* Instance = nullptr;
static UDataTable* BiomeGenerationDataTable = nullptr;

const static int32 RENDER_DISTANCE = 12;

// Sets default values
AChunkManager::AChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void AChunkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get Player Location
	const FVector PlayerLocation = GetFirstPlayerLocation();
	const FIntVector2 PlayerChunkLocation(PlayerLocation.X / (AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale()),
		PlayerLocation.Y / (AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale()));
	
	// TODO clean up out of range chunks
	TSet<FSpawnedChunkData> NewSpawnedChunks = SpawnedChunks;
	for (const FSpawnedChunkData& SpawnedChunkData : SpawnedChunks)
	{
		if (!IsValid(SpawnedChunkData.Chunk))
		{
			continue;
		}

		const int32 DistanceFromPlayer = SpawnedChunkData.Distance(PlayerChunkLocation);
		if (DistanceFromPlayer <= RENDER_DISTANCE)
		{
			continue;
		}

		// TODO save and unload
		FChunkData ChunkSaveData;
		SpawnedChunkData.Chunk->Unload(ChunkSaveData);

		// Be sure to overwrite the existing data for this chunk
		if (ChunkData.Contains(ChunkSaveData))
		{
			ChunkData.Remove(ChunkSaveData);
		}

		ChunkData.Add(ChunkSaveData);

		NewSpawnedChunks.Remove(SpawnedChunkData);
	}

	SpawnedChunks = NewSpawnedChunks;

	// TODO generate/load new in range chunks
	for (int32 RenderX = -RENDER_DISTANCE; RenderX <= RENDER_DISTANCE; ++RenderX)
	{
		for (int32 RenderY = -RENDER_DISTANCE; RenderY <= RENDER_DISTANCE; ++RenderY)
		{
			FSpawnedChunkData SpawnedChunkData;
			SpawnedChunkData.GridLocation = FIntVector2(RenderX, RenderY) + PlayerChunkLocation;
			if (SpawnedChunkData.Distance(PlayerChunkLocation) > RENDER_DISTANCE)
			{
				continue;
			}

			if (!SpawnedChunks.Contains(SpawnedChunkData))
			{
				// TODO check if we have existing data to load
				GenerateChunk(SpawnedChunkData);
				if (!IsValid(SpawnedChunkData.Chunk))
				{
					continue;
				}
				SpawnedChunks.Add(SpawnedChunkData);
			}
		}
	}
}

void AChunkManager::SetChunkData(const TSet<FChunkData> InChunkData)
{
	ChunkData = InChunkData;
}

TSet<FChunkData> AChunkManager::GetChunkData() const
{
	return ChunkData;
}

TSet<FSpawnedChunkData> AChunkManager::GetSpawnedChunkData() const
{
	return SpawnedChunks;
}

void AChunkManager::SetGenerationSeed(const uint32& Seed)
{
	AChunk::SetGenerationSeed(Seed);
}

// Called when the game starts or when spawned
void AChunkManager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr || World->IsEditorWorld() && IsValid(Instance))
	{
		return;
	}

	Instance = this;
}

void AChunkManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Instance = nullptr;
}

void AChunkManager::GenerateChunk(FSpawnedChunkData& OutSpawnedChunkData)
{
	const FVector SpawnLocation(OutSpawnedChunkData.GridLocation.X * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(),
		OutSpawnedChunkData.GridLocation.Y * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(), 0.0f);
	OutSpawnedChunkData.Chunk = GetWorld()->SpawnActor<AChunk>(ChunkClass, SpawnLocation, FRotator::ZeroRotator);
	if (!IsValid(OutSpawnedChunkData.Chunk))
	{
		OutSpawnedChunkData.Chunk = nullptr;
		return;
	}

	OutSpawnedChunkData.Chunk->Generate(OutSpawnedChunkData.GridLocation);
}

FVector AChunkManager::GetFirstPlayerLocation() const
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return FVector::ZeroVector;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return FVector::ZeroVector;
	}

	APawn* PlayerPawn = PlayerController->GetPawn();
	if (PlayerPawn == nullptr)
	{
		return FVector::ZeroVector;
	}

	return PlayerPawn->GetActorLocation();
}

TArray<FBiomeGenerationData*> AChunkManager::GetAllPossibleBiomes()
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

FBiomeGenerationData* AChunkManager::GetBiomeGenerationData(const FName& BiomeName)
{
	if (BiomeGenerationDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Biome Generation Data Context"));

	return BiomeGenerationDataTable->FindRow<FBiomeGenerationData>(BiomeName, ContextString, true);
}

AChunkManager* AChunkManager::GetChunkManager()
{
	return Instance;
}