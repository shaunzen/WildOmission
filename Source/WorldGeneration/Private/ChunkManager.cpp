// Copyright Telephone Studios. All Rights Reserved.


#include "ChunkManager.h"
#include "Actors/Chunk.h"
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
	FTimerHandle GenerationTimerHandle;
	FTimerDelegate GenerationTimerDelegate;
	GenerationTimerDelegate.BindUObject(this, &AChunkManager::GenerateChunks);
	GetWorld()->GetTimerManager().SetTimer(GenerationTimerHandle, GenerationTimerDelegate, 1.0f, false);	
}

void AChunkManager::AddChunk(AChunk* ChunkToAdd)
{
	Chunks.Add(ChunkToAdd);
}

void AChunkManager::RemoveChunk(AChunk* ChunkToRemove)
{
	Chunks.Remove(ChunkToRemove);
}

void AChunkManager::ClearChunks()
{
	Chunks.Empty();
}

TArray<class AChunk*> AChunkManager::GetChunks() const
{
	return Chunks;
}

UClass* AChunkManager::GetChunkClass() const
{
	return ChunkClass;
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

	if (OnWorldGenerationComplete.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("Broadcasting some shit."));
		OnWorldGenerationComplete.Broadcast();
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