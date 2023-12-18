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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DensityPerMeter = 1.0f;

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