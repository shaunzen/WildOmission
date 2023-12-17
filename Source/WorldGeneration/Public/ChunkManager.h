// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/BiomeGenerationData.h"
#include "Structs/ChunkData.h"
#include "Structs/SpawnedChunkData.h"
#include "ChunkManager.generated.h"

UCLASS()
class WORLDGENERATION_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkManager();

	virtual void Tick(float DeltaTime) override;

	void SetChunkData(const TArray<struct FChunkData> InChunkData);

	TArray<FChunkData> GetChunksData() const;
	bool GetChunkData(const FIntVector2& ChunkLocation, FChunkData& OutChunkData) const;

	TArray<FSpawnedChunkData> GetSpawnedChunks() const;
	bool GetSpawnedChunk(const FIntVector2& ChunkLocation, FSpawnedChunkData& OutSpawnedChunkData) const;

	static void SetGenerationSeed(const uint32& Seed);

	static TArray<FBiomeGenerationData*> GetAllPossibleBiomes();
	static FBiomeGenerationData* GetBiomeGenerationData(const FName& BiomeName);

	static AChunkManager* GetChunkManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TSubclassOf<class AChunk> ChunkClass;


	// Chunk Data contains stuff like, the grid location for query, the spawn data for actors on that chunk
	// shape of the terrain, if it has been generated or dormant, just about anything that the system needs 
	// to know to load/unload the chunk properly. The save system can load its save data into the FChunkData
	// format for the chunk manager to handle

	// Chunk data contains data for all chunks (save data weather an area was generated or not ect).
	TArray<FChunkData> ChunkData;
	
	// Spawned Chunk Data contains grid location information 
	// about spawned chunks, as well as a pointer to the spawned chunk object.
	TArray<FSpawnedChunkData> SpawnedChunks;

	// When Passing Chunk Data in, make sure to populate grid location, it is what will be used when generating the chunk.
	void SpawnChunk(FSpawnedChunkData& OutSpawnedChunkData);
	void GenerateChunk(const FSpawnedChunkData& InSpawnedChunkData);
	void LoadChunk(const FSpawnedChunkData& InSpawnedChunkData, const FChunkData& InChunkData);

	FVector GetFirstPlayerLocation() const;

};
