// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TornadoData.generated.h"

USTRUCT()
struct WEATHER_API FTornadoData
{
	GENERATED_BODY()

	FTornadoData();

	UPROPERTY(VisibleAnywhere, SaveGame)
	bool WasSpawned;

	UPROPERTY(VisibleAnywhere, SaveGame)
	FTransform Transform;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, SaveGame)
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float TotalLifetime;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float RemainingLifetime;

	// 0.0f = F0 
	// 5.0f = F5
	// >6.0f = Extreme Tornado (Rare)
	UPROPERTY(VisibleAnywhere, SaveGame)
	float Severity;

};