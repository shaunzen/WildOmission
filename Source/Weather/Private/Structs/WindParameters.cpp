// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/WindParameters.h"

FWindParameters::FWindParameters()
{
	GlobalWindStrength = 0.3f;
	GlobalWindDirection = FVector2D(1.0f, 0.0f);
	TornadoOnGround = 0.0f;
	TornadoLocation = FVector2D::ZeroVector;
}
