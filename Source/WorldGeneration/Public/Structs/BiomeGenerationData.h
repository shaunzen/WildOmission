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
	float DensityPerMeter = 0.00001f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool FollowSurfaceNormal = false;

};

USTRUCT(BlueprintType)
struct WORLDGENERATION_API FBiomeGenerationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSpawnData> Trees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSpawnData> Nodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSpawnData> Collectables;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FSpawnData> Lootables;

};