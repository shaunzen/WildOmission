// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/WorldGenerationSettings.h"
#include "Structs/BiomeGenerationData.h"
#include "Interfaces/WorldGenerator.h"
#include "WorldGenerationHandler.generated.h"

class ASaveHandler;

UCLASS()
class WORLDGENERATION_API AWorldGenerationHandler : public AActor, public IWorldGenerator
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGenerationHandler();

	// Begin IWorldGenerator Implementation
	virtual void GenerateLevel(ASaveHandler* InstigatingSaveHandler, UWildOmissionSaveGame* InSaveFile) override;
	// End IWorldGenerator Implementation

	FVector2D GetWorldSizeMeters();
	static FBiomeGenerationData* GetBiomeGenerationData(const FName& BiomeName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	ASaveHandler* SaveHandler;

	UFUNCTION()
	void Generate(const FWorldGenerationSettings& GenerationSettings);
	
	// TODO move regeneration to separate component
	UFUNCTION()
	void CheckNodeRegenerationConditions();
	
	UFUNCTION()
	void RegenerateNodesAroundOrigin(const FWorldGenerationSettings& GenerationSettings, const FVector& Origin);

	TArray<AActor*> FilterActorsByRange(const TArray<AActor*>& InList, const FVector& Origin, float Range);

	void GenerateTrees(const FWorldGenerationSettings& GenerationSettings);
	void GenerateNodes(const FWorldGenerationSettings& GenerationSettings);
	void GenerateCollectables(const FWorldGenerationSettings& GenerationSettings);
	void GenerateLootables(const FWorldGenerationSettings& GenerationSettings);

	bool FindSpawnLocation(const FWorldGenerationSettings& GenerationSettings, FVector& OutLocation, FVector& OutSurfaceNormal);
	bool FindSpawnTransform(const FWorldGenerationSettings& GenerationSettings, FTransform& OutTransform, bool FollowSurfaceNormal = false);
	bool FindSpawnTransformAroundOrigin(const FWorldGenerationSettings& GenerationSettings, const FVector& Origin, FTransform& OutTransform, bool FollowSurfaceNormal = false);
};
