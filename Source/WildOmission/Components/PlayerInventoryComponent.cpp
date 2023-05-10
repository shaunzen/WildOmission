// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerInventoryComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Core/WildOmissionStatics.h"
#include "WildOmission/Items/WorldItem.h"
#include "WildOmission/Items/EquipableItem.h"
#include "WildOmission/Components/EquipComponent.h"
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

	OwnerCharacter = Cast<AWildOmissionCharacter>(GetOwner());

	UInventoryManipulatorComponent* OwnerManipulator = OwnerCharacter->FindComponentByClass<UInventoryManipulatorComponent>();
	
	ToolbarSelectionIndex = -1;

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	OnUpdate.AddDynamic(this, &UPlayerInventoryComponent::RefreshToolbarSelectionState);

	if (LoadedFromSave == false)
	{
		FInventoryItem RockItem;
		RockItem.Name = FName("rock");
		RockItem.Quantity = 1;

		AddItem(RockItem);
	}
}

//**************************************************************
// General Management
//**************************************************************

void UPlayerInventoryComponent::RefreshPlayerEquip(FInventorySlot& SelectedSlot)
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}
	UEquipComponent* PlayerEquipComponent = OwnerCharacter->FindComponentByClass<UEquipComponent>();
	if (PlayerEquipComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RefreshPlayerEquip: Failed to get player's equip component"));
		return;
	}

	// return if there is no item
	if (SelectedSlot.IsEmpty())
	{
		PlayerEquipComponent->Disarm();
		return;
	}

	// get the equipable subclass for this item
	FItemData* SlotItemData = UWildOmissionStatics::GetItemData(SelectedSlot.Item.Name);
	if (SlotItemData == nullptr || SlotItemData->EquipItemClass == nullptr)
	{
		PlayerEquipComponent->Disarm();
		return;
	}

	AEquipableItem* CurrentEquipedItem = PlayerEquipComponent->GetEquipedItem();
	AEquipableItem* CurrentEquipedDefaultClass = PlayerEquipComponent->GetLocalEquipedItemDefaultClass();

	// is this item the same as we are already holding
	if (!OwnerCharacter->IsLocallyControlled() && CurrentEquipedItem && SlotItemData->EquipItemClass.Get() == CurrentEquipedItem->GetClass() && SelectedSlot.Item.UniqueID == CurrentEquipedItem->GetUniqueItemID())
	{
		return;
	}
	else if (OwnerCharacter->IsLocallyControlled() && CurrentEquipedDefaultClass && SlotItemData->EquipItemClass.Get() == CurrentEquipedDefaultClass->GetClass() && (CurrentEquipedItem && SelectedSlot.Item.UniqueID == CurrentEquipedItem->GetUniqueID()))
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

	RefreshPlayerEquip(SelectedSlot);
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