// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldGenerationSettings.generated.h"

USTRUCT()
struct FWorldGenerationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 WorldSizeMetersX = 2000;
	
	UPROPERTY(EditDefaultsOnly)
	int32 WorldSizeMetersY = 2000;
	
	UPROPERTY(EditDefaultsOnly)
	int32 WorldHeightMeters = 500;

	UPROPERTY(EditDefaultsOnly)
	float SpawnLimiter = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 MinNodeCount = 30;

	UPROPERTY(EditDefaultsOnly)
	int32 MinCollectableCount = 20;

};
