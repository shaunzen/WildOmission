// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerInventoryComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Items/WorldItem.h"
#include "WildOmission/Components/EquipComponent.h"
#include "Net/UnrealNetwork.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	SlotCount = 30;
	ToolbarSelectionIndex = -1;
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AWildOmissionCharacter>(GetOwner());
	
	UInventoryManipulatorComponent* OwnerManipulator = OwnerCharacter->FindComponentByClass<UInventoryManipulatorComponent>();
	
	SetManipulator(OwnerManipulator);

	ToolbarSelectionIndex = -1;
}

void UPlayerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerInventoryComponent, ToolbarSelectionIndex);
}

//**************************************************************
// General Management
//**************************************************************

void UPlayerInventoryComponent::RefreshPlayerEquip(FInventorySlot& SelectedSlot)
{
	UEquipComponent* PlayerEquipComponent = OwnerCharacter->FindComponentByClass<UEquipComponent>();
	if (PlayerEquipComponent == nullptr)
	{
		return;
	}

	// return if there is no item
	if (SelectedSlot.IsEmpty())
	{
		PlayerEquipComponent->Disarm();
		return;
	}

	// get the equipable subclass for this item
	FItem* SlotItemData = GetItemData(SelectedSlot.Item.Name);
	if (SlotItemData == nullptr || SlotItemData->EquipItemClass == nullptr)
	{
		PlayerEquipComponent->Disarm();
		return;
	}

	AEquipableItem* CurrentEquipedItem = PlayerEquipComponent->GetEquipedItem();

	// is this item the same as we are already holding
	if (CurrentEquipedItem && SelectedSlot.Index == CurrentEquipedItem->GetFromSlotIndex() && SlotItemData->EquipItemClass.Get() == CurrentEquipedItem->GetClass())
	{
		return;
	}

	PlayerEquipComponent->Disarm();
	PlayerEquipComponent->EquipItem(SlotItemData->EquipItemClass, SelectedSlot.Index);

}

//**************************************************************
// User Interaction
//**************************************************************

void UPlayerInventoryComponent::IncrementToolbarSelection()
{
	Server_SetToolbarSelectionIndex(ToolbarSelectionIndex + 1);
	RefreshUI();
}

void UPlayerInventoryComponent::DecrementToolbarSelection()
{
	Server_SetToolbarSelectionIndex(ToolbarSelectionIndex - 1);
	RefreshUI();
}

void UPlayerInventoryComponent::SetToolbarSelectionIndex(const int8& SelectionIndex)
{
	Server_SetToolbarSelectionIndex(SelectionIndex);
	RefreshUI();
}

void UPlayerInventoryComponent::RemoveHeldItem()
{
	Server_RemoveHeldItem();
	RefreshUI();
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

void UPlayerInventoryComponent::OnInventoryChange()
{
	Super::OnInventoryChange();

	RefreshToolbarSelectionState();
}

void UPlayerInventoryComponent::RefreshToolbarSelectionState()
{
	if (!IsToolbarSlotSelectionValid())
	{
		return;
	}

	FInventorySlot& SelectedSlot = Slots[ToolbarSelectionIndex];

	RefreshPlayerEquip(SelectedSlot);
}

bool UPlayerInventoryComponent::IsToolbarSlotSelectionValid() const
{
	return ToolbarSelectionIndex > -1 && ToolbarSelectionIndex < 6;
}

//**************************************************************
// RPC
//**************************************************************

void UPlayerInventoryComponent::Server_SetToolbarSelectionIndex_Implementation(int8 SelectionIndex)
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

	RefreshToolbarSelectionState();
}

void UPlayerInventoryComponent::Server_RemoveHeldItem_Implementation()
{
	FInventorySlot& SelectedSlot = Slots[ToolbarSelectionIndex];

	--SelectedSlot.Item.Quantity;

	if (SelectedSlot.IsEmpty())
	{
		SelectedSlot.ClearItem();
	}

	RefreshToolbarSelectionState();
}