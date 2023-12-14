// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/BiomeGenerationData.h"
#include "Structs/ChunkData.h"
#include "ChunkManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWorldGenerationCompleteSignature);

UCLASS()
class WORLDGENERATION_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkManager();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Generate();

	FOnWorldGenerationCompleteSignature OnWorldGenerationComplete;

	void AddChunk(class AChunk* ChunkToAdd);
	void RemoveChunk(class AChunk* ChunkToRemove);
	void ClearChunks();

	TSet<class AChunk*> GetChunks() const;
	UClass* GetChunkClass() const;

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

	TSet<class AChunk*> SpawnedChunks;

	// Chunk Data contains stuff like, the grid location for query, the spawn data for actors on that chunk
	// shape of the terrain, if it has been generated or dormant, just about anything that the system needs 
	// to know to load/unload the chunk properly. The save system can load its save data into the FChunkData
	// format for the chunk manager to handle
	TSet<FChunkData> ChunkData;

	FVector GetFirstPlayerLocation() const;

	UFUNCTION()
	void GenerateChunks();

};
