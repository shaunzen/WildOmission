// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/BiomeGenerationData.h"
#include "SpawnQuery.generated.h"

USTRUCT()
struct FSpawnQuery
{
	GENERATED_BODY()

	FSpawnQuery();
	FSpawnQuery(const FSpawnData& InSpawnData, float InTestValue);

	UPROPERTY()
	FSpawnData SpawnData;

	UPROPERTY()
	float TestValue;

};