// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/WorldGenerationSettings.h"
#include "Structs/BiomeGenerationData.h"
#include "Interfaces/WorldGenerator.h"
#include "WorldGenerationHandler.generated.h"

class UResourceRegenerationComponent;
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

	static bool FindSpawnTransformRandomLocation(UWorld* WorldContextObject, FTransform& OutTransform, const FWorldGenerationSettings& GenerationSettings, bool FollowSurfaceNormal = false);

	// Origin = The Starting Location, InnerRadius = The Closest Possible Spawn, OuterRadius = The Farthest Possible Spawn
	static bool FindSpawnTransformRadiusFromOrigin(UWorld* WorldContextObject, FTransform& OutTransform, const FVector& Origin, float InnerRadius, float OuterRadius, const FWorldGenerationSettings& GenerationSettings, bool FollowSurfaceNormal = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UResourceRegenerationComponent* RegenerationComponent;

	UPROPERTY()
	ASaveHandler* SaveHandler;

	UFUNCTION()
	void Generate(const FWorldGenerationSettings& GenerationSettings);
	
	void GenerateResource(const TArray<FSpawnData>& SpawnData, const FWorldGenerationSettings& GenerationSettings, bool FollowSurfaceNormal = false);
	
};
