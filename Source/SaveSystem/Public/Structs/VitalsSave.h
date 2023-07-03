// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VitalsSave.generated.h"

USTRUCT()
struct SAVESYSTEM_API FVitalsSave
{
	GENERATED_BODY()

	UPROPERTY()
	float Health = 50.0f;

	UPROPERTY()
	float Hunger = 100.0f;

	UPROPERTY()
	float Thirst = 100.0f;

};