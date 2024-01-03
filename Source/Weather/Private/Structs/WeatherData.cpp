// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/WeatherData.h"

FStormData::FStormData()
{
	ByteData = TArray<uint8>();
}

FWeatherData::FWeatherData()
{
	StormData = TArray<FStormData>();
}