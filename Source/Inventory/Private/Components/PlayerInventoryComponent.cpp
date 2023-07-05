// Copyright Telephone Studios. All Rights Reserved.


#include "Components/PlayerInventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "WorldItem.h"
#include "Net/UnrealNetwork.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	SetIsReplicatedByDefault(true);

	SlotCount = 30;
	ToolbarSelectionIndex = -1;
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UInventoryManipulatorComponent* OwnerManipulator = GetOwner()->FindComponentByClass<UInventoryManipulatorComponent>();
	
	ToolbarSelectionIndex = -1;

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	OnUpdate.AddDynamic(this, &UPlayerInventoryComponent::RefreshToolbarSelectionState);

	if (!WasLoadedFromSave())
	{
		FInventoryItem RockItem;
		RockItem.Name = FName("rock");
		RockItem.Quantity = 1;

		AddItem(RockItem);
	}
}

void UPlayerInventoryComponent::OnRep_ServerState()
{
	Super::OnRep_ServerState();

	RefreshToolbarSelectionState();
}

//**************************************************************
// User Interaction
//**************************************************************

void UPlayerInventoryComponent::IncrementToolbarSelection()
{
	SetToolbarSelectionIndex(ToolbarSelectionIndex + 1);
}

void UPlayerInventoryComponent::DecrementToolbarSelection()
{
	SetToolbarSelectionIndex(ToolbarSelectionIndex - 1);
}

void UPlayerInventoryComponent::SetToolbarSelectionIndex(int8 SelectionIndex)
{
	if (SelectionIndex == -2 || SelectionIndex > 5)
	{
		SelectionIndex = 0;
	}
	else if (SelectionIndex < 0)
	{
		SelectionIndex = 5;
	}

	ToolbarSelectionIndex = SelectionIndex;

	Server_SetToolbarSelectionIndex(ToolbarSelectionIndex);

	RefreshToolbarSelectionState();
	BroadcastInventoryUpdate();
}

void UPlayerInventoryComponent::RemoveHeldItem()
{
	FInventorySlot& SelectedSlot = ServerState.Slots[ToolbarSelectionIndex];

	ServerState.Contents.RemoveItem(SelectedSlot.Item.Name, 1);

	--SelectedSlot.Item.Quantity;

	if (SelectedSlot.IsEmpty())
	{
		SelectedSlot.ClearItem();
	}

	RefreshToolbarSelectionState();
	OnRep_ServerState();
}

//**************************************************************
// Getters
//**************************************************************

int8 UPlayerInventoryComponent::GetToolbarSelectionIndex()
{
	return ToolbarSelectionIndex;
}

//**************************************************************
// Slot Functions
//**************************************************************

void UPlayerInventoryComponent::RefreshToolbarSelectionState()
{
	if (!OnToolbarSlotSelectionChange.IsBound() || !IsToolbarSlotSelectionValid())
	{
		return;
	}

	FInventorySlot& SelectedSlot = ServerState.Slots[ToolbarSelectionIndex];
	OnToolbarSlotSelectionChange.Broadcast(ToolbarSelectionIndex, SelectedSlot);
}

bool UPlayerInventoryComponent::IsToolbarSlotSelectionValid() const
{
	return ToolbarSelectionIndex > -1 && ToolbarSelectionIndex < 6;
}

//**************************************************************
// RPC
//**************************************************************

bool UPlayerInventoryComponent::Server_SetToolbarSelectionIndex_Validate(int8 SelectionIndex)
{
	return SelectionIndex >= 0 && SelectionIndex <= 5;
}

void UPlayerInventoryComponent::Server_SetToolbarSelectionIndex_Implementation(int8 SelectionIndex)
{
	ToolbarSelectionIndex = SelectionIndex;

	RefreshToolbarSelectionState();
}