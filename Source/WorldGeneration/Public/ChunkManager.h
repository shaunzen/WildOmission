// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/BiomeGenerationData.h"
#include "ChunkManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWorldGenerationCompleteSignature);

UCLASS()
class WORLDGENERATION_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkManager();

	UFUNCTION()
	void Generate();

	FOnWorldGenerationCompleteSignature OnWorldGenerationComplete;

	void AddChunk(class AChunk* ChunkToAdd);
	void RemoveChunk(class AChunk* ChunkToRemove);
	void ClearChunks();

	TArray<class AChunk*> GetChunks() const;
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

	TArray<class AChunk*> Chunks;

	UFUNCTION()
	void GenerateChunks();

};
