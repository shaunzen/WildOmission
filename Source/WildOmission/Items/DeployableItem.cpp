// Copyright Telephone Studios. All Rights Reserved.


#include "DeployableItem.h"
#include "Engine/StaticMeshActor.h"

void ADeployableItem::Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerCharacter, InItemName, InFromSlotIndex, InUniqueID);
	
	// show preview of item we are spawning
	Client_SpawnPreview();
}

void ADeployableItem::OnUnequip()
{
	Super::OnUnequip();

	// destroy preview
	Client_DestroyPreview();
}

void ADeployableItem::Primary()
{
	Super::Primary();

	// spawn on the server the real deployable item
	// remove this current item from our inventory
}

void ADeployableItem::Client_SpawnPreview_Implementation()
{
	// if not null destroy
	// spawn preview actor
}

void ADeployableItem::Client_DestroyPreview_Implementation()
{
	// if null return
	// destroy preview actor
}