// Copyright Telephone Studios. All Rights Reserved.


#include "DeployableItem.h"

void ADeployableItem::Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerCharacter, InItemName, InFromSlotIndex, InUniqueID);
	
	// show preview of item we are spawning
}

void ADeployableItem::OnUnequip()
{
	Super::OnUnequip();

	// destroy preview
}

void ADeployableItem::Primary()
{
	Super::Primary();

	// spawn on the server the real deployable item
	// remove this current item from our inventory
}