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

	// is this item the same as we are already holding
	if (CurrentEquipedItem && SlotItemData->EquipItemClass.Get() == CurrentEquipedItem->GetClass() && SelectedSlot.Item.UniqueID == CurrentEquipedItem->GetUniqueItemID())
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
	Server_SetToolbarSelectionIndex(ToolbarSelectionIndex + 1);
	BroadcastInventoryUpdate();
}

void UPlayerInventoryComponent::DecrementToolbarSelection()
{
	Server_SetToolbarSelectionIndex(ToolbarSelectionIndex - 1);
	BroadcastInventoryUpdate();
}

void UPlayerInventoryComponent::SetToolbarSelectionIndex(const int8& SelectionIndex)
{
	Server_SetToolbarSelectionIndex(SelectionIndex);
	BroadcastInventoryUpdate();
}

void UPlayerInventoryComponent::RemoveHeldItem()
{
	FInventorySlot& SelectedSlot = Slots[ToolbarSelectionIndex];

	--SelectedSlot.Item.Quantity;

	if (SelectedSlot.IsEmpty())
	{
		SelectedSlot.ClearItem();
	}

	RefreshToolbarSelectionState();

	BroadcastInventoryUpdate();
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