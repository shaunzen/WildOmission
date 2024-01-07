// Copyright Telephone Studios. All Rights Reserved.


#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

static AChunkManager* Instance = nullptr;
static UDataTable* BiomeGenerationDataTable = nullptr;

const static int32 RENDER_DISTANCE = 16;

// Sets default values
AChunkManager::AChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

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

	if (!HasAuthority())
	{
		return;
	}

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

void AChunkManager::SaveAllLoadedChunks()
{
	for (const FSpawnedChunk& SpawnedChunk : SpawnedChunks)
	{
		SaveChunkData(SpawnedChunk.Chunk);
	}
}

void AChunkManager::SaveChunkData(AChunk* ChunkToSave, bool AlsoDestroy)
{
	FChunkData ChunkSaveData;
	ChunkToSave->Save(ChunkSaveData, AlsoDestroy);

	const int32 ChunkIndex = ChunkData.Find(ChunkSaveData);
	if (ChunkIndex == INDEX_NONE)
	{
		ChunkData.Add(ChunkSaveData);
		return;
	}

	ChunkData[ChunkIndex] = ChunkSaveData;
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

void AChunkManager::AddSpawnedChunk(const FSpawnedChunk& InSpawnedChunk)
{
	if (!IsValid(InSpawnedChunk.Chunk))
	{
		return;
	}

	SpawnedChunks.Add(InSpawnedChunk);
}

void AChunkManager::RemoveSpawnedChunk(const FSpawnedChunk& InSpawnedChunk)
{
	SpawnedChunks.Remove(InSpawnedChunk);
}

AChunk* AChunkManager::GenerateChunkAtLocation(const FIntVector2& ChunkLocation)
{
	FSpawnedChunk SpawnedChunk;
	SpawnedChunk.GridLocation = ChunkLocation;

	const int32 SpawnedIndex = SpawnedChunks.Find(SpawnedChunk);
	if (SpawnedIndex == INDEX_NONE)
	{
		SpawnChunk(SpawnedChunk);

		if (!IsValid(SpawnedChunk.Chunk))
		{
			return nullptr;
		}

		SpawnedChunks.Add(SpawnedChunk);

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

		return SpawnedChunk.Chunk;
	}

	return SpawnedChunks[SpawnedIndex].Chunk;
}

FVector AChunkManager::GetWorldSpawnPoint()
{
	const float ChunkSize = AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale();

	const int32 TestRadius = 128;
	FIntVector2 ValidChunk(0, 0);
	for (int32 X = -TestRadius; X < TestRadius; ++X)
	{
		for (int32 Y = -TestRadius; Y < TestRadius; ++Y)
		{
			const FVector2D TestLocation(
				X * ChunkSize,
				Y * ChunkSize
			);
			const float Continentalness = AChunk::GetContinentalnessAtLocation(TestLocation, true);
			if (Continentalness > 0.0f || Continentalness < -0.1f)
			{
				// If not pick a new starting point and keep going
				continue;
			}

			ValidChunk = FIntVector2(X, Y);
			break;
		}
	}

	// Line trace down to find ground
	FHitResult HitResult;

	// Ensure the chunk we want to start at exists
	(void*)GenerateChunkAtLocation(FIntVector2(ValidChunk.X, ValidChunk.Y));
	const FVector ValidChunkWorldLocation((ValidChunk.X * ChunkSize) - (ChunkSize * 0.5f), (ValidChunk.Y * ChunkSize) - (ChunkSize * 0.5f), 0.0f);

	// Loop through all points on chunk to see if there is a good spot
	for (int32 X = 0; X <= AChunk::GetVertexSize(); ++X)
	{
		for (int32 Y = 0; Y <= AChunk::GetVertexSize(); ++Y)
		{
			const FVector Start((X * AChunk::GetVertexDistanceScale()) + ValidChunkWorldLocation.X, (Y * AChunk::GetVertexDistanceScale()) + ValidChunkWorldLocation.Y, AChunk::GetMaxHeight());
			const FVector End(Start.X, Start.Y, -AChunk::GetMaxHeight());
			if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
			{
				continue;
			}

			UPrimitiveComponent* HitComponent = HitResult.GetComponent();
			if (HitComponent == nullptr || !HitComponent->ComponentHasTag(TEXT("Ground")))
			{
				continue;
			}

			break;
		}
	}

	return HitResult.ImpactPoint;
}

uint8 AChunkManager::GetSurfaceTypeAtLocation(const FVector& TestLocation) const
{
	const int32 ChunkSize = AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale();
	const FIntVector2 TestChunkLocation(
		FMath::RoundToInt32(TestLocation.X / ChunkSize),
		FMath::RoundToInt32(TestLocation.Y / ChunkSize));

	FSpawnedChunk SpawnedChunk;
	SpawnedChunk.GridLocation = TestChunkLocation;
	const int32 SpawnedChunkIndex = SpawnedChunks.Find(SpawnedChunk);
	if (SpawnedChunkIndex == INDEX_NONE)
	{
		return 0;
	}
	
	const AChunk* TestChunk = SpawnedChunks[SpawnedChunkIndex].Chunk;
	if (TestChunk == nullptr)
	{
		// TODO this is causing the snow to break
		return 0;
	}

	// Get surface index at point at which we are testing for within this chunk
	const float ChunkHalfSize = ChunkSize * 0.5f;
	const float AX = ((TestLocation.X - ChunkHalfSize) / ChunkSize);
	const float AY = ((TestLocation.Y - ChunkHalfSize) / ChunkSize);
	const int32 TestX = (AX - FMath::Floor(AX)) * AChunk::GetVertexSize();
	const int32 TestY = (AY - FMath::Floor(AY)) * AChunk::GetVertexSize();

	const int32 ArrayIndex = (TestX * (AChunk::GetVertexSize() + 1)) + TestY;

	const TArray<uint8> SurfaceData = TestChunk->GetSurfaceData();
	if (!SurfaceData.IsValidIndex(ArrayIndex))
	{
		return 0;
	}

	return SurfaceData[ArrayIndex];
}

FBiomeGenerationData* AChunkManager::GetBiomeAtLocation(const FVector& TestLocation)
{
	return AChunk::GetBiomeAtLocation(FVector2D(TestLocation.X, TestLocation.Y));
}

float AChunkManager::GetRenderDistanceCentimeters()
{
	return AChunk::GetVertexSize() * (AChunk::GetVertexDistanceScale() * 0.5f) * RENDER_DISTANCE;
}

int32 AChunkManager::GetRenderDistance()
{
	return RENDER_DISTANCE;
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

			FVector PlayerLocation = FVector::ZeroVector;
			FRotator PlayerRotation = FRotator::ZeroRotator;
			PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

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

		SaveChunkData(SpawnedChunk.Chunk, true);
	}

	SpawnedChunks = NewSpawnedChunks;
}

void AChunkManager::SpawnChunksForPlayer(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))
	{
		return;
	}

	// Get Player Location
	FVector PlayerLocation = FVector::ZeroVector;
	FRotator PlayerRotation = FRotator::ZeroRotator;
	PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

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

				SpawnedChunks.Add(SpawnedChunk);

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

			}
		}
	}
}

void AChunkManager::SpawnChunk(FSpawnedChunk& OutSpawnedChunk) const
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

void AChunkManager::GenerateChunk(const FSpawnedChunk& InSpawnedChunk) const
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

void AChunkManager::LoadChunk(const FSpawnedChunk& InSpawnedChunk, const FChunkData& InChunkData) const
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