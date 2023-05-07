// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryManipulatorComponent.h"
#include "WildOmission/Items/WorldItem.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Core/WildOmissionStatics.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryManipulatorComponent::UInventoryManipulatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Dragging = false;
}


// Called when the game starts
void UInventoryManipulatorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	Dragging = false;
	SelectedItem.Clear();
}

//**************************************************************
// General Management
//**************************************************************

void UInventoryManipulatorComponent::StartDragging(const FInventoryItem& ItemToDrag)
{
	SelectedItem = ItemToDrag;
	Dragging = true;

	BroadcastSelectionChanged();
}

void UInventoryManipulatorComponent::StopDragging()
{
	if (Dragging == false)
	{
		return;
	}

	SelectedItem.Clear();
	Dragging = false;

	BroadcastSelectionChanged();
}

//**************************************************************
// User Interaction
//**************************************************************

void UInventoryManipulatorComponent::Server_DropSelectedItemInWorld_Implementation(bool Single)
{
	if (!Dragging)
	{
		return;
	}
	
	FInventoryItem ItemToSpawn;
	ItemToSpawn = SelectedItem;

	if (Single == true)
	{
		ItemToSpawn.Quantity = 1;
		UWildOmissionStatics::SpawnWorldItem(GetWorld(), ItemToSpawn, GetOwner());
		SelectedItem.Quantity -= 1;
	}
	else
	{
		UWildOmissionStatics::SpawnWorldItem(GetWorld(), ItemToSpawn, GetOwner());
		SelectedItem.Clear();
	}

	if (SelectedItem.Quantity == 0)
	{
		StopDragging();
	}

}

//**************************************************************
// Getters
//**************************************************************

bool UInventoryManipulatorComponent::IsDragging() const
{
	return Dragging;
}

bool UInventoryManipulatorComponent::SelectedItemHasUniqueID(const uint32& UniqueID) const
{
	if (Dragging == false || SelectedItem.UniqueID != UniqueID)
	{
		return false;
	}

	return true;
}

FInventoryItem UInventoryManipulatorComponent::GetSelectedItem()
{
	return SelectedItem;
}

FInventoryItem* UInventoryManipulatorComponent::GetSelectedItemAddress()
{
	return &SelectedItem;
}

void UInventoryManipulatorComponent::BroadcastSelectionChanged()
{
	if (!OnSelectionChanged.IsBound())
	{
		return;
	}

	OnSelectionChanged.Broadcast(SelectedItem);
}