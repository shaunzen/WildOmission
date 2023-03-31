// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "WildOmission/Resources/HarvestableResource.h"
#include "BiomeGenerationData.generated.h"

// TODO one of these for collectables too
USTRUCT(BlueprintType)
struct FHarvestableResourceData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Identifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AHarvestableResource> BlueprintClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DensityPerTenMeters = 0.0f;

};

USTRUCT(BlueprintType)
struct FBiomeGenerationData : public FTableRowBase
{
	GENERATED_BODY()
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FHarvestableResourceData> Trees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FHarvestableResourceData> Nodes;

};