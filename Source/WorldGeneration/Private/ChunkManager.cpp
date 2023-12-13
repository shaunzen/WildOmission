// Copyright Telephone Studios. All Rights Reserved.


#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static AChunkManager* Instance = nullptr;
static UDataTable* BiomeGenerationDataTable = nullptr;

const static uint8 RENDER_DISTANCE = 12;

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

	FChunkPosition PlayerCurrentChunk(PlayerLocation.X / AChunk::GetVertexDistanceScale(), PlayerLocation.Y / AChunk::GetVertexDistanceScale());

	if (!GeneratedChunks.Contains(PlayerCurrentChunk))
	{
		const FVector ChunkLocation(PlayerCurrentChunk.X * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(), PlayerCurrentChunk.Y * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale(), 0.0f);
		AChunk* SpawnedChunk = GetWorld()->SpawnActor<AChunk>(ChunkClass, ChunkLocation, FRotator::ZeroRotator);
		SpawnedChunk->Generate(FIntVector2(PlayerCurrentChunk.X, PlayerCurrentChunk.Y));
		Chunks.Add(SpawnedChunk);

		GeneratedChunks.Add(PlayerCurrentChunk);
	}

	//const int32 WorldSize = 50;

}

// Called when the game starts or when spawned
void AChunkManager::BeginPlay()
{
	Super::BeginPlay();

	const int32 Seed = FMath::RandRange(0, 999999999);
	AChunk::SetGenerationSeed(Seed);

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

void AChunkManager::GenerateChunks()
{


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