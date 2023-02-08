// Copyright Telephone Studios. All Rights Reserved.


#include "FoodItem.h"

AFoodItem::AFoodItem()
{
	Energy = 100.0f;
}

void AFoodItem::Primary()
{
	Super::Primary();
	
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Orange, FString::Printf(TEXT("Food Energy: %i"), Energy));

	// remove one of our type from the players inventory
	// refresh the players current held item
	// add our energy amount to the players hunger bar
	// play some kind of eat sound
}

uint8 AFoodItem::GetEnergy()
{
	return Energy;
}

void AFoodItem::SetEnergy(uint8 InEnergy)
{
	Energy = InEnergy;
}