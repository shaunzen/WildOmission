// Copyright Telephone Studios. All Rights Reserved.


#include "ToolItem.h"

AToolItem::AToolItem()
{
	ToolType = EToolType::WOOD;

	GatherMultiplyer = 1.0f;
	EffectiveRangeCentimeters = 2000.0f;
	SwingTimeSeconds = 1.0f;
}

void AToolItem::Primary()
{
	Super::Primary();

}

void AToolItem::Secondary()
{
	Super::Secondary();

}