// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/WeatherSave.h"

FStormData::FStormData()
{
	ByteData = TArray<uint8>();
}

FWeatherData::FWeatherData()
{
	NextStormSpawnTime = 0.0f;
	StormData = TArray<FStormData>();
}