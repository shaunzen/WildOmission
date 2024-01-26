// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WindParameters.generated.h"

USTRUCT()
struct FWindParameters
{
	GENERATED_BODY()

	FWindParameters();

	UPROPERTY()
	float GlobalWindStrength;

	UPROPERTY()
	FVector2D GlobalWindDirection;

	UPROPERTY()
	float TornadoOnGround;

	UPROPERTY()
	FVector2D TornadoLocation;

};