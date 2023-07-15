// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/BuildingPart.h"

ABuildingPart::ABuildingPart()
{
	Stability = 100.0f;
}

float ABuildingPart::GetStability() const
{
	return Stability;
}
