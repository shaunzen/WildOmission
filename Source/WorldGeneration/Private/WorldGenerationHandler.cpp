// Copyright Telephone Studios. All Rights Reserved.


#include "WorldGenerationHandler.h"
#include "Chunk.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static AWorldGenerationHandler* Instance = nullptr;
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

	static ConstructorHelpers::FClassFinder<AChunk> ChunkBlueprint(TEXT("/Game/WorldGeneration/BP_Chunk"));
	if (ChunkBlueprint.Succeeded())
	{
		ChunkClass = ChunkBlueprint.Class;
	}
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

void AWorldGenerationHandler::Generate()
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
			Chunks.Add(SpawnedChunk);
			// TODO add to chunk list for keeping track of
		}
	}
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