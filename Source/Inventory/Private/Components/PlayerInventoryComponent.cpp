// Copyright Telephone Studios. All Rights Reserved.


#include "Components/PlayerInventoryComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Core/WildOmissionStatics.h"
#include "WildOmission/Items/WorldItem.h"
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
// General Management
//**************************************************************

void UPlayerInventoryComponent::RefreshPlayerEquip(FInventorySlot& SelectedSlot)
{
	UEquipComponent* PlayerEquipComponent = OwnerCharacter->FindComponentByClass<UEquipComponent>();
	if (PlayerEquipComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RefreshPlayerEquip: Failed to get player's equip component"));
		return;
	}

	// return if there is no item
	if (SelectedSlot.IsEmpty())
	{	

		return;
	}

	// get the equipable subclass for this item
	FItemData* SlotItemData = UInventoryComponent::GetItemData(SelectedSlot.Item.Name);
	if (SlotItemData == nullptr || SlotItemData->EquipItemClass == nullptr)
	{
		PlayerEquipComponent->Disarm();
		return;
	}

	AEquipableItem* CurrentEquipedItem = PlayerEquipComponent->GetEquipedItem();
	AEquipableItem* CurrentEquipedDefaultClass = PlayerEquipComponent->GetLocalEquipedItemDefaultClass();

	// is this item the same as we are already holding
	if (OwnerCharacter->HasAuthority() && CurrentEquipedItem && SlotItemData->EquipItemClass.Get() == CurrentEquipedItem->GetClass() && SelectedSlot.Item.UniqueID == CurrentEquipedItem->GetUniqueItemID())
	{
		return;
	}

	// Locallized version of same item check
	if (OwnerCharacter->IsLocallyControlled() && CurrentEquipedDefaultClass && SlotItemData->EquipItemClass.Get() == CurrentEquipedDefaultClass->GetClass() && SelectedSlot.Item.UniqueID == CurrentEquipedDefaultClass->GetUniqueItemID())
	{
		return;
	}

	PlayerEquipComponent->Disarm();
	PlayerEquipComponent->EquipItem(SelectedSlot.Item.Name, SlotItemData->EquipItemClass, SelectedSlot.Index, SelectedSlot.Item.UniqueID);

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
	if (!IsToolbarSlotSelectionValid())
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