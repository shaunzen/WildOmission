// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/WorldGenerationSettings.h"
#include "Structs/BiomeGenerationData.h"
#include "WorldGenerationHandler.generated.h"

class UPreventExtinctionComponent;
class UResourceRegenerationComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGenerationCompleteSignature);

UCLASS()
class WORLDGENERATION_API AWorldGenerationHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGenerationHandler();

	void GenerateLevel();
	FOnGenerationCompleteSignature OnGenerationComplete;

	FVector2D GetWorldSizeMeters();
	static TArray<FBiomeGenerationData*> GetAllPossibleBiomes();
	static FBiomeGenerationData* GetBiomeGenerationData(const FName& BiomeName);

	static AWorldGenerationHandler* GetWorldGenerationHandler();

	static bool FindSpawnTransformRandomLocation(UWorld* WorldContextObject, FTransform& OutTransform, const FWorldGenerationSettings& GenerationSettings, bool FollowSurfaceNormal = false);

	// Origin = The Starting Location, InnerRadius = The Closest Possible Spawn, OuterRadius = The Farthest Possible Spawn
	static bool FindSpawnTransformRadiusFromOrigin(UWorld* WorldContextObject, FTransform& OutTransform, const FVector& Origin, float InnerRadius, float OuterRadius, const FWorldGenerationSettings& GenerationSettings, bool FollowSurfaceNormal = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	UPreventExtinctionComponent* PreventExtinctionComponent;

	UPROPERTY(VisibleAnywhere)
	UResourceRegenerationComponent* RegenerationComponent;

	UFUNCTION()
	void Generate(const FWorldGenerationSettings& GenerationSettings);
	
	void GenerateResource(const TArray<FSpawnData>& SpawnData, const FWorldGenerationSettings& GenerationSettings, bool FollowSurfaceNormal = false);
	
};
