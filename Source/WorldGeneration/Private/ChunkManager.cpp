// Copyright Telephone Studios. All Rights Reserved.


#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static AChunkManager* Instance = nullptr;
static UDataTable* BiomeGenerationDataTable = nullptr;

const static int32 RENDER_DISTANCE = 32;

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

	RemoveOutOfRangeChunks();

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		SpawnChunksForPlayer(Iterator->Get());
	}
}

void AChunkManager::SetChunkData(const TArray<FChunkData> InChunkData)
{
	ChunkData = InChunkData;
}

TArray<FChunkData> AChunkManager::GetChunksData() const
{
	return ChunkData;
}

bool AChunkManager::GetChunkData(const FIntVector2& ChunkLocation, FChunkData& OutChunkData) const
{
	FChunkData ChunkDataLocator;
	ChunkDataLocator.GridLocation = ChunkLocation;

	const int32 ChunkIndex = ChunkData.Find(ChunkDataLocator);
	if (ChunkIndex == INDEX_NONE)
	{
		return false;
	}

	OutChunkData = ChunkData[ChunkIndex];
	return true;
}

TArray<FSpawnedChunkData> AChunkManager::GetSpawnedChunks() const
{
	return SpawnedChunks;
}

bool AChunkManager::GetSpawnedChunk(const FIntVector2& ChunkLocation, FSpawnedChunkData& OutSpawnedChunkData) const
{
	FSpawnedChunkData ChunkDataLocator;
	ChunkDataLocator.GridLocation = ChunkLocation;

	const int32 ChunkIndex = SpawnedChunks.Find(ChunkDataLocator);
	if (ChunkIndex == INDEX_NONE)
	{
		return false;
	}

	OutSpawnedChunkData = SpawnedChunks[ChunkIndex];
	return true;
}

// TODO use this for all chunk generation
void AChunkManager::GenerateChunkAtLocation(const FIntVector2& ChunkLocation)
{
	FSpawnedChunkData SpawnedChunkData;
	SpawnedChunkData.GridLocation = ChunkLocation;

	if (!SpawnedChunks.Contains(SpawnedChunkData))
	{
		SpawnChunk(SpawnedChunkData);

		if (!IsValid(SpawnedChunkData.Chunk))
		{
			return;
		}

		// Check if we have existing data to load
		FChunkData ChunkSaveData;
		ChunkSaveData.GridLocation = SpawnedChunkData.GridLocation;
		const int32 SaveIndex = ChunkData.Find(ChunkSaveData);
		if (SaveIndex != INDEX_NONE)
		{
			LoadChunk(SpawnedChunkData, ChunkData[SaveIndex]);
		}
		else
		{
			GenerateChunk(SpawnedChunkData);
		}

		SpawnedChunks.Add(SpawnedChunkData);
	}
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

void AChunkManager::RemoveOutOfRangeChunks()
{
	TArray<FSpawnedChunkData> NewSpawnedChunks = SpawnedChunks;
	for (const FSpawnedChunkData& SpawnedChunkData : SpawnedChunks)
	{
		if (!IsValid(SpawnedChunkData.Chunk))
		{
			continue;
		}
		
		int32 DistanceFromClosestPlayer = 0;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (!IsValid(PlayerController))
			{
				continue;
			}

			APawn* Pawn = PlayerController->GetPawn();
			if (!IsValid(Pawn))
			{
				continue;
			}

			const FVector PlayerLocation = Pawn->GetActorLocation();

			const FIntVector2 PlayerChunkLocation(PlayerLocation.X / (AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale()),
				PlayerLocation.Y / (AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale()));

			const int32 DistanceFromPlayer = SpawnedChunkData.Distance(PlayerChunkLocation);
			if (DistanceFromClosestPlayer > 0 && DistanceFromPlayer > DistanceFromClosestPlayer)
			{
				continue;
			}

			DistanceFromClosestPlayer = DistanceFromPlayer;
		}

		if (DistanceFromClosestPlayer <= RENDER_DISTANCE)
		{
			continue;
		}

		FChunkData ChunkSaveData;
		SpawnedChunkData.Chunk->Save(ChunkSaveData, true);

		// TODO Be sure to overwrite the existing data for this chunk
		// so that we can prevent memory leak
		if (ChunkData.Contains(ChunkSaveData))
		{
			ChunkData.Remove(ChunkSaveData);
		}

		ChunkData.Add(ChunkSaveData);

		NewSpawnedChunks.Remove(SpawnedChunkData);
	}

	SpawnedChunks = NewSpawnedChunks;
}

void AChunkManager::SpawnChunksForPlayer(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))
	{
		return;
	}

	APawn* Pawn = PlayerController->GetPawn();
	if (!IsValid(Pawn))
	{
		return;
	}

	// Get Player Location
	const FVector PlayerLocation = Pawn->GetActorLocation();
	
	// Uncomment for generation stats
	/*const FVector2D TwoDimensionPlayerLocation(PlayerLocation.X, PlayerLocation.Y);
	UE_LOG(LogTemp, Warning, TEXT("Location %s"), *TwoDimensionPlayerLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("C %f, Raw C %f"),
		AChunk::GetContinentalnessAtLocation(TwoDimensionPlayerLocation), AChunk::GetContinentalnessAtLocation(TwoDimensionPlayerLocation, true));
	UE_LOG(LogTemp, Warning, TEXT("E %f, Raw E %f"),
		AChunk::GetErosionAtLocation(TwoDimensionPlayerLocation), AChunk::GetErosionAtLocation(TwoDimensionPlayerLocation, true));
	UE_LOG(LogTemp, Warning, TEXT("P&V %f, Raw P&V %f"),
		AChunk::GetPeaksAndValleysAtLocation(TwoDimensionPlayerLocation), AChunk::GetPeaksAndValleysAtLocation(TwoDimensionPlayerLocation, true));*/

	const FIntVector2 PlayerChunkLocation(PlayerLocation.X / (AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale()),
		PlayerLocation.Y / (AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale()));

	
	// Generate/load new in range chunks
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
				SpawnChunk(SpawnedChunkData);

				if (!IsValid(SpawnedChunkData.Chunk))
				{
					continue;
				}

				// TODO check if we have existing data to load
				FChunkData ChunkSaveData;
				ChunkSaveData.GridLocation = SpawnedChunkData.GridLocation;
				const int32 SaveIndex = ChunkData.Find(ChunkSaveData);
				if (SaveIndex != INDEX_NONE)
				{
					LoadChunk(SpawnedChunkData, ChunkData[SaveIndex]);
				}
				else
				{
					GenerateChunk(SpawnedChunkData);
				}

				SpawnedChunks.Add(SpawnedChunkData);
			}
		}
	}
}

void AChunkManager::SpawnChunk(FSpawnedChunkData& OutSpawnedChunkData)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	const FVector SpawnLocation(OutSpawnedChunkData.GridLocation.X * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(),
		OutSpawnedChunkData.GridLocation.Y * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(), 0.0f);
	OutSpawnedChunkData.Chunk = World->SpawnActor<AChunk>(ChunkClass, SpawnLocation, FRotator::ZeroRotator);
	if (!IsValid(OutSpawnedChunkData.Chunk))
	{
		return;
	}

	OutSpawnedChunkData.Chunk->SetChunkLocation(FIntVector2(OutSpawnedChunkData.GridLocation.X, OutSpawnedChunkData.GridLocation.Y));
}

void AChunkManager::GenerateChunk(const FSpawnedChunkData& InSpawnedChunkData)
{
	if (!IsValid(InSpawnedChunkData.Chunk))
	{
		return;
	}

	InSpawnedChunkData.Chunk->Generate();
}

void AChunkManager::LoadChunk(const FSpawnedChunkData& InSpawnedChunkData, const FChunkData& InChunkData)
{
	if (!IsValid(InSpawnedChunkData.Chunk))
	{
		return;
	}

	InSpawnedChunkData.Chunk->Load(InChunkData);
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