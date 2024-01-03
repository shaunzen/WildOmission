// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeatherSave.generated.h"

USTRUCT()
struct WEATHER_API FStormData
{
	GENERATED_BODY()

	FStormData();
	
	TArray<uint8> ByteData;

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