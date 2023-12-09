// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/BiomeGenerationData.h"
#include "WorldGenerationHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGenerationCompleteSignature);

UCLASS()
class WORLDGENERATION_API AWorldGenerationHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGenerationHandler();

	UFUNCTION()
	void Generate();

	FOnGenerationCompleteSignature OnGenerationComplete;

	static TArray<FBiomeGenerationData*> GetAllPossibleBiomes();
	static FBiomeGenerationData* GetBiomeGenerationData(const FName& BiomeName);

	static AWorldGenerationHandler* GetWorldGenerationHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TSubclassOf<class AChunk> ChunkClass;

	TArray<AChunk*> Chunks;

	void GenerateChunks();

};
