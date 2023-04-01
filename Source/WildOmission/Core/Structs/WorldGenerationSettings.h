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
	int32 WorldSizeMetersX = 500;
	
	UPROPERTY(EditDefaultsOnly)
	int32 WorldSizeMetersY = 500;
	
	UPROPERTY(EditDefaultsOnly)
	int32 WorldHeightMeters = 100;

	UPROPERTY(EditDefaultsOnly)
	float TreeDensity = 0.005f;
	
	UPROPERTY(EditDefaultsOnly)
	float NodeDensity = 0.003f;
};
