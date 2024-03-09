// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/ChunkData.h"
#include "Structs/SpawnedChunk.h"
#include "Structs/PlayerSpawnChunk.h"
#include "ChunkManager.generated.h"

UCLASS()
class WORLDGENERATION_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkManager();


	virtual void Tick(float DeltaTime) override;

	// Gets existing chunk if present, or returns the generated/loaded chunk at the given location
	class AChunk* GetChunkAtLocation(const FIntVector2& ChunkLocation);
	class AChunk* GetChunkAtLocation(const FVector& ChunkLocation);
	static void HandleActorRenderDistanceVisibility(UWorld* WorldContextObject, AActor* InActor);
	static bool IsActorNetRelevent(const AActor* ActorToTest, const AActor* ViewingActor);
	void ClearDecorationsAroundChunk(const FIntVector2& Origin, const FIntVector2& Size);
	void FlattenTerrainAroundChunk(const FIntVector2& Origin, const FIntVector2& Size, float DesiredHeight);

	// Chunk Data	
	void SetChunkData(const TArray<FChunkData> InChunkData);
	TArray<FChunkData> GetAllChunkData() const;
	bool GetChunkData(const FIntVector2& ChunkLocation, FChunkData& OutChunkData) const;

	// Spawned Chunk
	void SaveAllSpawnedChunks();
	TArray<FSpawnedChunk> GetAllSpawnedChunks() const;
	bool GetSpawnedChunk(const FIntVector2& ChunkLocation, FSpawnedChunk& OutSpawnedChunk) const;
	void AddSpawnedChunk(const FSpawnedChunk& InSpawnedChunk);
	void RemoveSpawnedChunk(const FSpawnedChunk& InSpawnedChunk);

	// Surface/Biome
	uint8 GetSurfaceTypeAtLocation(const FVector& TestLocation) const;
	static FName GetBiomeNameAtLocation(const FVector& TestLocation);

	// Seed
	static void SetGenerationSeed(const uint32& Seed);
	static uint32 GetGenerationSeed();

	FVector GetWorldSpawnPoint();
	FPlayerSpawnChunk GetPlayerSpawnChunk() const;
	void SetPlayerSpawnChunk(const FPlayerSpawnChunk& InPlayerSpawnChunk);

	static void SetChunkManager(AChunkManager* NewInstance);
	static AChunkManager* GetChunkManager();

	UFUNCTION()
	void SpawnChunksAtLocation(const FVector& Location, const uint8& RenderDistance);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TSubclassOf<class AChunk> ChunkClass;

	// Array of all chunk data
	TArray<FChunkData> ChunkData;
	
	// Array of all currently spawned chunks
	TArray<FSpawnedChunk> SpawnedChunks;

	FPlayerSpawnChunk PlayerSpawnChunk;

	void SaveChunkData(AChunk* ChunkToSave, bool AlsoDestroy = false);

	UFUNCTION()
	void RemoveOutOfRangeChunks(const TArray<class UChunkInvokerComponent*>& ChunkInvokers);
	UFUNCTION()
	void SpawnInRangeChunks(const TArray<class UChunkInvokerComponent*>& ChunkInvokers);

	// Generates a chunk at the location pre populated in the struct
	// sets structs chunk to the chunk spawned if successful
	void SpawnChunk(FSpawnedChunk& OutSpawnedChunkData) const;

	// Handles generation of the passed SpawnedChunk
	void GenerateChunk(const FSpawnedChunk& InSpawnedChunk) const;

	// Handles loading data on the passed SpawnedChunk
	void LoadChunk(const FSpawnedChunk& InSpawnedChunk, const FChunkData& InChunkData) const;

};
