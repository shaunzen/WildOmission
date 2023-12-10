// Copyright Telephone Studios. All Rights Reserved.


#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Structs/ChunkSaveData.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static AChunkManager* Instance = nullptr;
static UDataTable* BiomeGenerationDataTable = nullptr;

// Sets default values
AChunkManager::AChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

void AChunkManager::Generate()
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	GenerateChunks();

	if (OnGenerationComplete.IsBound())
	{
		OnGenerationComplete.Broadcast();
	}
}

void AChunkManager::Save(TArray<FChunkSaveData>& ChunkData)
{
	ChunkData.Empty();

	for (AChunk* Chunk : Chunks)
	{
		if (!IsValid(Chunk))
		{
			continue;
		}

		FChunkSaveData SaveData;

		FMemoryWriter MemoryWriter(SaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;

		Chunk->Serialize(Archive);

		SaveData.GridLocation = Chunk->GetChunkLocation();
		
		ChunkData.Add(SaveData);
	}
}

void AChunkManager::Load(const TArray<FChunkSaveData>& ChunkData)
{

	for (const FChunkSaveData& SaveData : ChunkData)
	{
		const FVector ChunkLocation(SaveData.GridLocation.X * 1600.0f, SaveData.GridLocation.Y * 1600.0f, 0.0f);
		AChunk* SpawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunkClass, ChunkLocation, FRotator::ZeroRotator);

		// TODO Load values from save
		FMemoryReader MemoryReader(SaveData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
		Archive.ArIsSaveGame = true;

		SpawnedChunk->Serialize(Archive);

		SpawnedChunk->OnLoadFromSaveComplete();

		Chunks.Add(SpawnedChunk);
	}
	
}

void AChunkManager::GenerateChunks()
{
	const int32 WorldSize = 10;
	const int32 Seed = FMath::RandRange(0, 999999999);
	AChunk::SetGenerationSeed(999999999);
	
	UE_LOG(LogTemp, Warning, TEXT("Generating Terrain with seed %i"), Seed);

	for (int32 X = 0; X < WorldSize; ++X)
	{
		for (int32 Y = 0; Y < WorldSize; ++Y)
		{
			// generate chunk
			const FVector ChunkLocation(X * 1600.0f, Y * 1600.0f, 0.0f);
			AChunk* SpawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunkClass, ChunkLocation, FRotator::ZeroRotator);
			SpawnedChunk->Generate(FIntVector2(X, Y));
			Chunks.Add(SpawnedChunk);
		}
	}
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