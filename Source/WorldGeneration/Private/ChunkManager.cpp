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
	// TODO generate chunks
	
	for (int32 RenderX = -RENDER_DISTANCE; RenderX <= RENDER_DISTANCE; ++RenderX)
	{
		for (int32 RenderY = -RENDER_DISTANCE; RenderY <= RENDER_DISTANCE; ++RenderY)
		{
			FSpawnedChunkData SpawnedChunkData;
			SpawnedChunkData.GridLocation = FIntVector2(RenderX, RenderY) + PlayerChunkLocation;
			if (!SpawnedChunks.Contains(SpawnedChunkData))
			{
				GenerateChunk(SpawnedChunkData);
				SpawnedChunks.Add(SpawnedChunkData);
			}
		}
	}



	//FChunkPosition PlayerCurrentChunk(PlayerLocation.X / (AChunk::GetVertexDistanceScale() * AChunk::GetVertexSize()), PlayerLocation.Y / (AChunk::GetVertexDistanceScale() * AChunk::GetVertexSize()));
	//// this is terrible, please optimize the hell out of this
	//for (int32 RenderX = -RENDER_DISTANCE; RenderX <= RENDER_DISTANCE; ++RenderX)
	//{
	//	for (int32 RenderY = -RENDER_DISTANCE; RenderY <= RENDER_DISTANCE; ++RenderY)
	//	{
	//		const FChunkPosition RenderChunk(RenderX + PlayerCurrentChunk.X, RenderY + PlayerCurrentChunk.Y);
	//		if (!GeneratedChunks.Contains(RenderChunk))
	//		{
	//			const FVector ChunkLocation(RenderChunk.X * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(), RenderChunk.Y * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(), 0.0f);
	//			UE_LOG(LogTemp, Warning, TEXT("Chunk Location: %s"), *ChunkLocation.ToString());
	//			AChunk* SpawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunkClass, ChunkLocation, FRotator::ZeroRotator);
	//			SpawnedChunk->Generate(FIntVector2(RenderChunk.X, RenderChunk.Y));
	//			Chunks.Add(SpawnedChunk);

	//			GeneratedChunks.Add(RenderChunk);
	//		}
	//	}
	//}

	////const FChunkPosition OutOfRangeTopRight(PlayerCurrentChunk.X + -RENDER_DISTANCE, PlayerCurrentChunk.Y + RENDER_DISTANCE);
	////const FChunkPosition OutOfRangeBottomLeft(PlayerCurrentChunk.X + RENDER_DISTANCE, PlayerCurrentChunk.Y + -RENDER_DISTANCE);
	//const int32 MinX = PlayerCurrentChunk.X - RENDER_DISTANCE;
	//const int32 MinY = PlayerCurrentChunk.Y - RENDER_DISTANCE;
	//const int32 MaxX = PlayerCurrentChunk.X + RENDER_DISTANCE;
	//const int32 MaxY = PlayerCurrentChunk.Y + RENDER_DISTANCE;

	//UE_LOG(LogTemp, Warning, TEXT("MinX %i, MinY %i, MaxX %i, MaxY %i"), MinX, MinY, MaxX, MaxY);

	//for (const FChunkPosition& ChunkPosition : GeneratedChunks)
	//{
	//	if (ChunkPosition.X > MinX && ChunkPosition.X < MaxX &&
	//		ChunkPosition.Y > MinY && ChunkPosition.Y < MaxY)
	//	{
	//		continue;
	//	}
	//	
	//	// this is an out of range chunk, find and remove it!
	//	for (AChunk* Chunk : Chunks)
	//	{
	//		if (!IsValid(Chunk) || (Chunk->GetChunkLocation().X != ChunkPosition.X && Chunk->GetChunkLocation().Y != ChunkPosition.Y))
	//		{
	//			continue;
	//		}
	//		//GeneratedChunks.Remove(ChunkPosition);
	//		Chunk->Destroy();
	//	}
	//}
	//UE_LOG(LogTemp, Warning, TEXT("Player Chunk Location: %s"), *FString::Printf(TEXT("X: %i, Y: %i"), PlayerCurrentChunk.X, PlayerCurrentChunk.Y));

	////const int32 WorldSize = 50;

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