// Copyright Telephone Studios. All Rights Reserved.


#include "FoodItem.h"
#include "WildOmission/Player/WildOmissionCharacter.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Components/VitalsComponent.h"

AFoodItem::AFoodItem()
{
	Energy = 100.0f;
}

void AFoodItem::Primary()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	Super::Primary();

	OwnerCharacter->GetVitalsComponent()->AddHunger(Energy);

	// TODO play eat sound

	OwnerCharacter->GetInventoryComponent()->RemoveHeldItem();
}

uint8 AFoodItem::GetEnergy()
{
	return Energy;
}

void AFoodItem::SetEnergy(uint8 InEnergy)
{
	Energy = InEnergy;
}