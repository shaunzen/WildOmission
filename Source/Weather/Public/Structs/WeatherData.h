// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeatherData.generated.h"

USTRUCT()
struct WEATHER_API FStormData
{
	GENERATED_BODY()

	FStormData();

	UPROPERTY()
	TArray<uint8> ByteData;

};

USTRUCT()
struct WEATHER_API FWeatherData
{
	GENERATED_BODY()

	FWeatherData();

	UPROPERTY()
	TArray<FStormData> StormData;

};