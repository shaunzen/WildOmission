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
	int32 WorldSizeX = 10000;
	
	UPROPERTY(EditDefaultsOnly)
	int32 WorldSizeY = 10000;
	
	UPROPERTY(EditDefaultsOnly)
	int32 WorldHeight = 1000;

	UPROPERTY(EditDefaultsOnly)
	float TreeDensity = 0.00005f;
	
	UPROPERTY(EditDefaultsOnly)
	float NodeDensity = 0.00003f;
};
