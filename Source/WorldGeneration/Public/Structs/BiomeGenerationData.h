// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "BiomeGenerationData.generated.h"

USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> BlueprintClass;

	// Min attempts to spawn per chunk
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MinCountPerChunk = 0;

	// Max attempts to spawn per chunk, not guaranteed unless spawn chance is 1
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxCountPerChunk = 3;

	// 1 = 100% chance of spawning, 0.5 = 50% chance of spawning, 0 = 0% chance of spawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SpawnChance = 1.0f;

	// TODO this is deprecated as of now
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool FollowSurfaceNormal = false;

};

USTRUCT(BlueprintType)
struct WORLDGENERATION_API FBiomeGenerationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MajorHeightScale = 0.005f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinorHeightScale = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RoughnessHeightScale = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MajorHeightMultiplier = 1000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinorHeightMultiplier = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RoughnessHeightMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSpawnData> Trees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSpawnData> Nodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSpawnData> Collectables;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSpawnData> Lootables;

};