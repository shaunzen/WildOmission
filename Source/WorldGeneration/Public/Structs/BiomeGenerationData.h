// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Actors/HarvestableResource.h"
#include "Actors/CollectableResource.h"
#include "BiomeGenerationData.generated.h"


USTRUCT(BlueprintType)
struct FHarvestableResourceData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AHarvestableResource> BlueprintClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DensityPerMeter = 0.001f;

};

USTRUCT(BlueprintType)
struct FCollectableResourceData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ACollectableResource> BlueprintClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DensityPerMeter = 0.0005f;
};

USTRUCT(BlueprintType)
struct FLootableData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> BlueprintClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DensityPerMeter = 0.00001f;

};

USTRUCT(BlueprintType)
struct WORLDGENERATION_API FBiomeGenerationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FHarvestableResourceData> Trees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FHarvestableResourceData> Nodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FCollectableResourceData> Collectables;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FLootableData> Lootables;

};