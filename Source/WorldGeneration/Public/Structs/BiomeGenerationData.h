// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "BiomeGenerationData.generated.h"

USTRUCT(BlueprintType)
struct FSpawnableNoiseParameters
{
	GENERATED_BODY()

	// Lower Number = bigger clusters of spawning, with more gaps
	// Setting number too high may prevent spawn unless tolerance is increased
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float NoiseScale = 0.5f;

	// The +- tolerance that the noise will use to determine if spawn should be attemped
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Tolerance = 0.1f;

};

USTRUCT(BlueprintType)
struct FSpawnableData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> BlueprintClass;

	// 1 = 100% chance of spawning, 0.5 = 50% chance of spawning, 0 = 0% chance of spawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SpawnChance = 0.75f;

	// This is deprecated as of now
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool FollowSurfaceNormal = false;

};

USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSpawnableNoiseParameters NoiseParameters;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSpawnableData> Spawnables;
};

USTRUCT(BlueprintType)
struct WORLDGENERATION_API FBiomeGenerationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSpawnData Trees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSpawnData Nodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSpawnData Collectables;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSpawnData Lootables;

};