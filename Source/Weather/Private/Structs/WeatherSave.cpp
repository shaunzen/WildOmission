// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/WeatherSave.h"

FTornadoData::FTornadoData()
{
	WasSpawned = false;
	Transform = FTransform();
	RotationSpeed = 0.0f;
	MovementSpeed = 0.0f;
	TargetLocation = FVector::ZeroVector;
	TotalLifetime = 0.0f;
	RemainingLifetime = 0.0f;
}

FStormData::FStormData()
{
	SpawnLocation = FVector::ZeroVector;
	MovementVector = FVector::ZeroVector;
	MovementSpeed = 0.0f;
	SeverityMultiplier = 0.0f;
	Severity = 0.0f;
	TornadoData = FTornadoData();
	HasSpawnedTornado = false;
}

FWeatherData::FWeatherData()
{
	NextStormSpawnTime = 0.0f;
	StormData = TArray<FStormData>();
}