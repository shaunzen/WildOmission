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

TArray<FSpawnedChunk> AChunkManager::GetSpawnedChunks() const
{
	return SpawnedChunks;
}

bool AChunkManager::GetSpawnedChunk(const FIntVector2& ChunkLocation, FSpawnedChunk& OutSpawnedChunk) const
{
	FSpawnedChunk SpawnedChunkLocator;
	SpawnedChunkLocator.GridLocation = ChunkLocation;

	const int32 ChunkIndex = SpawnedChunks.Find(SpawnedChunkLocator);
	if (ChunkIndex == INDEX_NONE)
	{
		return false;
	}

	OutSpawnedChunk = SpawnedChunks[ChunkIndex];
	return true;
}

void AChunkManager::GenerateChunkAtLocation(const FIntVector2& ChunkLocation)
{
	FSpawnedChunk SpawnedChunk;
	SpawnedChunk.GridLocation = ChunkLocation;

	if (!SpawnedChunks.Contains(SpawnedChunk))
	{
		SpawnChunk(SpawnedChunk);

		if (!IsValid(SpawnedChunk.Chunk))
		{
			return;
		}

		// Check if we have existing data to load
		FChunkData ChunkSaveData;
		ChunkSaveData.GridLocation = SpawnedChunk.GridLocation;
		const int32 SaveIndex = ChunkData.Find(ChunkSaveData);
		if (SaveIndex != INDEX_NONE)
		{
			LoadChunk(SpawnedChunk, ChunkData[SaveIndex]);
		}
		else
		{
			GenerateChunk(SpawnedChunk);
		}

		SpawnedChunks.Add(SpawnedChunk);
	}
}
void AChunkManager::SetGenerationSeed(const uint32& Seed)
{
	AChunk::SetGenerationSeed(Seed);
}

uint32 AChunkManager::GetGenerationSeed()
{
	return AChunk::GetGenerationSeed();
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
	// This seems to destroy and replace chunks over and over every frame?
	// I need to figure out why

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	TArray<FSpawnedChunk> NewSpawnedChunks = SpawnedChunks;
	for (const FSpawnedChunk& SpawnedChunk : SpawnedChunks)
	{
		if (!IsValid(SpawnedChunk.Chunk))
		{
			continue;
		}
		
		int32 DistanceFromClosestPlayer = -1;

		// Gets the distance from closest player
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
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

			const int32 DistanceFromPlayer = SpawnedChunk.Distance(PlayerChunkLocation);
			if (DistanceFromClosestPlayer >= 0 && DistanceFromPlayer > DistanceFromClosestPlayer)
			{
				continue;
			}

			DistanceFromClosestPlayer = DistanceFromPlayer;
		}

		if (DistanceFromClosestPlayer == INDEX_NONE || DistanceFromClosestPlayer <= RENDER_DISTANCE)
		{
			continue;
		}

		// TODO does this automatically shrink?
		NewSpawnedChunks.Remove(SpawnedChunk);

		FChunkData ChunkSaveData;
		SpawnedChunk.Chunk->Save(ChunkSaveData, true);
		
		const int32 ChunkIndex = ChunkData.Find(ChunkSaveData);
		if (ChunkIndex == INDEX_NONE)
		{
			ChunkData.Add(ChunkSaveData);
			continue;
		}
		
		ChunkData[ChunkIndex] = ChunkSaveData;
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

	// Uncomment for generation stats
	/*const FVector2D TwoDimensionPlayerLocation(PlayerLocation.X, PlayerLocation.Y);
	UE_LOG(LogTemp, Warning, TEXT("Location %s"), *TwoDimensionPlayerLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("C %f, Raw C %f"),
		AChunk::GetContinentalnessAtLocation(TwoDimensionPlayerLocation), AChunk::GetContinentalnessAtLocation(TwoDimensionPlayerLocation, true));
	UE_LOG(LogTemp, Warning, TEXT("E %f, Raw E %f"),
		AChunk::GetErosionAtLocation(TwoDimensionPlayerLocation), AChunk::GetErosionAtLocation(TwoDimensionPlayerLocation, true));
	UE_LOG(LogTemp, Warning, TEXT("P&V %f, Raw P&V %f"),
		AChunk::GetPeaksAndValleysAtLocation(TwoDimensionPlayerLocation), AChunk::GetPeaksAndValleysAtLocation(TwoDimensionPlayerLocation, true));*/

	// Get Player Location
	const FVector PlayerLocation = Pawn->GetActorLocation();
	
	const FIntVector2 PlayerChunkLocation(PlayerLocation.X / (AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale()),
		PlayerLocation.Y / (AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale()));

	
	// Generate/load new in range chunks
	for (int32 RenderX = -RENDER_DISTANCE; RenderX <= RENDER_DISTANCE; ++RenderX)
	{
		for (int32 RenderY = -RENDER_DISTANCE; RenderY <= RENDER_DISTANCE; ++RenderY)
		{
			FSpawnedChunk SpawnedChunk;
			SpawnedChunk.GridLocation = FIntVector2(RenderX, RenderY) + PlayerChunkLocation;
			if (SpawnedChunk.Distance(PlayerChunkLocation) > RENDER_DISTANCE)
			{
				continue;
			}

			if (!SpawnedChunks.Contains(SpawnedChunk))
			{
				SpawnChunk(SpawnedChunk);

				if (!IsValid(SpawnedChunk.Chunk))
				{
					continue;
				}

				// TODO check if we have existing data to load
				FChunkData ChunkSaveData;
				ChunkSaveData.GridLocation = SpawnedChunk.GridLocation;
				const int32 SaveIndex = ChunkData.Find(ChunkSaveData);
				if (SaveIndex != INDEX_NONE)
				{
					LoadChunk(SpawnedChunk, ChunkData[SaveIndex]);
				}
				else
				{
					GenerateChunk(SpawnedChunk);
				}

				SpawnedChunks.Add(SpawnedChunk);
			}
		}
	}
}

void AChunkManager::SpawnChunk(FSpawnedChunk& OutSpawnedChunk)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	const FVector SpawnLocation(OutSpawnedChunk.GridLocation.X * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(),
		OutSpawnedChunk.GridLocation.Y * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(), 0.0f);
	OutSpawnedChunk.Chunk = World->SpawnActor<AChunk>(ChunkClass, SpawnLocation, FRotator::ZeroRotator);
	if (!IsValid(OutSpawnedChunk.Chunk))
	{
		return;
	}

	OutSpawnedChunk.Chunk->SetChunkLocation(FIntVector2(OutSpawnedChunk.GridLocation.X, OutSpawnedChunk.GridLocation.Y));
}

void AChunkManager::GenerateChunk(const FSpawnedChunk& InSpawnedChunk)
{
	if (!IsValid(InSpawnedChunk.Chunk))
	{
		return;
	}



	
	// We need to get neighbors from save file, because they might not be spawned when we are generating this chunk

	// Neighbors
	TArray<FChunkData> NeighborsData{
		// Top
		InSpawnedChunk.GridLocation + FIntVector2(0, 1),
		// Bottom
		InSpawnedChunk.GridLocation + FIntVector2(0, -1),
		// Left
		InSpawnedChunk.GridLocation + FIntVector2(-1, 0),
		// Right
		InSpawnedChunk.GridLocation + FIntVector2(1, 0),
		// Top Left
		InSpawnedChunk.GridLocation + FIntVector2(-1, 1),
		// Top Right
		InSpawnedChunk.GridLocation + FIntVector2(1, 1),
		// Bottom Left
		InSpawnedChunk.GridLocation + FIntVector2(-1, -1),
		// Bottom Right
		InSpawnedChunk.GridLocation + FIntVector2(1, -1)
	};
	
	for (FChunkData& NeighborData : NeighborsData)
	{
		const int32 NeighborIndex = ChunkData.Find(NeighborData);

		if (NeighborIndex == INDEX_NONE)
		{
			continue;
		}

		NeighborData = ChunkData[NeighborIndex];
	}

	// Pass neighbors
	InSpawnedChunk.Chunk->Generate(NeighborsData);
}

void AChunkManager::LoadChunk(const FSpawnedChunk& InSpawnedChunk, const FChunkData& InChunkData)
{
	if (!IsValid(InSpawnedChunk.Chunk))
	{
		return;
	}

	InSpawnedChunk.Chunk->Load(InChunkData);
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