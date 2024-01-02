// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeatherSave.generated.h"

USTRUCT()
struct FTornadoData
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

};

USTRUCT()
struct WEATHER_API FStormData
{
	GENERATED_BODY()

	FStormData();

	UPROPERTY()
	FVector SpawnLocation;

	UPROPERTY()
	FVector MovementVector;

	UPROPERTY()
	float MovementSpeed;

	UPROPERTY()
	float SeverityMultiplier;

	UPROPERTY()
	float Severity;

	UPROPERTY()
	FTornadoData TornadoData;

	UPROPERTY()
	bool HasSpawnedTornado;

};

USTRUCT()
struct WEATHER_API FWeatherData
{
	GENERATED_BODY()

	FWeatherData();

	UPROPERTY()
	float NextStormSpawnTime;

	TArray<FStormData> StormData;

};