// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/BiomeGenerationData.h"
#include "Structs/ChunkData.h"
#include "Structs/SpawnedChunkData.h"
#include "ChunkManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetAllChunksPendingUnloadSignature, bool, Unload);

UCLASS()
class WORLDGENERATION_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkManager();

	virtual void Tick(float DeltaTime) override;

	void SetChunkData(const TSet<struct FChunkData> InChunkData);
	TSet<FChunkData> GetChunkData() const;

	TSet<FSpawnedChunkData> GetSpawnedChunkData() const;

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
	TSet<FChunkData> ChunkData;
	
	// Spawned Chunk Data contains grid location information 
	// about spawned chunks, as well as a pointer to the spawned chunk object.
	TSet<FSpawnedChunkData> SpawnedChunks;

	// When Passing Chunk Data in, make sure to populate grid location, it is what will be used when generating the chunk.
	void GenerateChunk(FSpawnedChunkData& OutSpawnedChunkData);

	FSetAllChunksPendingUnloadSignature SetAllChunksPendingUnload;

	FVector GetFirstPlayerLocation() const;

};
