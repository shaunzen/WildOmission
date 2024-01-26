// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/TornadoData.h"

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
