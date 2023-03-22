// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryComponent.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);
	
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT("/Game/WildOmission/Core/DataTables/DT_Items"));
	if (ItemDataTableObject.Succeeded())
	{
		ItemDataTable = ItemDataTableObject.Object;
	}

	SlotCount = 24;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Contents);
	DOREPLIFETIME(UInventoryComponent, Slots);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	// Initialize slots
	Slots.SetNum(SlotCount, false);
	for (int32 i = 0; i < SlotCount; ++i)
	{
		Slots[i].Index = i;
	}
}

void UInventoryComponent::SetManipulator(UInventoryManipulatorComponent* InventoryManipulator)
{
	Manipulator = InventoryManipulator;
}

void UInventoryComponent::AddItem(const FInventoryItem& ItemToAdd)
{
	// The amount of items we were unable to add
	int32 Remaining;
	
	// The amount of items we successfully added
	int32 AmountAdded;
	
	bool AddSuccess = AddItemToSlots(ItemToAdd, Remaining);

	// Calculate how many were added and add that amount to our contents
	AmountAdded = ItemToAdd.Quantity - Remaining;
	Contents.AddItem(ItemToAdd.Name, AmountAdded);

	if (AddSuccess == false)
	{
		FInventoryItem DroppedItem = ItemToAdd;
		DroppedItem.Quantity = Remaining;
		Manipulator->SpawnWorldItem(DroppedItem);
	}

	OnInventoryChange();
}

void UInventoryComponent::RemoveItem(const FInventoryItem& ItemToRemove)
{
	int32 Remaining;
	int32 AmountSuccessfullyRemoved;
	
	RemoveItemFromSlots(ItemToRemove.Name, ItemToRemove.Quantity, Remaining);
	AmountSuccessfullyRemoved = ItemToRemove.Quantity - Remaining;

	// Remove from contents
	Contents.RemoveItem(ItemToRemove.Name, AmountSuccessfullyRemoved);

	// Call inventory change
	OnInventoryChange();
}

void UInventoryComponent::Server_SlotInteraction_Implementation(const int32& SlotIndex, bool Primary)
{
	// This is gross
	if (!Manipulator->IsDragging())
	{
		if (Primary)
		{
			DragAll(SlotIndex);
		}
		else
		{
			DragSplit(SlotIndex);
		}
	}
	else
	{
		if (Primary)
		{
			DropAll(SlotIndex);
		}
		else
		{
			DropSingle(SlotIndex);
		}
	}

	RefreshUI();
}

FItemData* UInventoryComponent::GetItemData(const FName& ItemName)
{
	if (ItemDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Item Data Context"));
	
	return ItemDataTable->FindRow<FItemData>(ItemName, ContextString, true);
}

FInventoryItem* UInventoryComponent::FindItemWithUniqueID(const uint32& UniqueID)
{
	FInventoryItem* FoundItem = nullptr;

	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.IsEmpty())
		{
			continue;
		}
		if (Slot.Item.UniqueID != UniqueID)
		{
			continue;
		}
		
		FoundItem = &Slot.Item;
		break;
	}

	return FoundItem;
}

FInventorySlot* UInventoryComponent::FindSlotContainingItem(const FName& ItemToFind)
{
	FInventorySlot* FoundSlot = nullptr;

	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.IsEmpty())
		{
			continue;
		}
		if (Slot.Item.Name != ItemToFind)
		{
			continue;
		}

		FoundSlot = &Slot;
		break;
	}

	return FoundSlot;
}

FInventoryContents* UInventoryComponent::GetContents()
{
	return &Contents;
}

FInventorySlot* UInventoryComponent::GetSlot(const int32& SlotIndex)
{
	return &Slots[SlotIndex];
}

TArray<FInventorySlot>& UInventoryComponent::GetSlots()
{
	return Slots;
}

UInventoryManipulatorComponent* UInventoryComponent::GetManipulator() const
{
	return Manipulator;
}

FWildOmissionInventorySave UInventoryComponent::Save()
{
	FWildOmissionInventorySave Save;

	// Save Contents List
	Save.Items = Contents.Contents;
	
	// Save Slots
	Save.Slots = Slots;

	return Save;
}

void UInventoryComponent::Load(const FWildOmissionInventorySave& InInventorySave)
{
	Contents.Contents = InInventorySave.Items;
	Slots = InInventorySave.Slots;
}

void UInventoryComponent::OnInventoryChange()
{
	RefreshUI();
}

void UInventoryComponent::RefreshUI()
{
	if (Manipulator == nullptr)
	{
		return;
	}

	AWildOmissionCharacter* ManipulatorCharacter = Cast<AWildOmissionCharacter>(Manipulator->GetOwner());
	if (ManipulatorCharacter == nullptr)
	{
		return;
	}

	UPlayerHUDWidget* ManipulatorHUD = ManipulatorCharacter->GetHUDWidget();
	if (ManipulatorHUD == nullptr)
	{
		return;
	}

	ManipulatorHUD->RefreshAllMenus();
}

bool UInventoryComponent::AddItemToSlots(const FInventoryItem& ItemToAdd, int32& Remaining)
{
	FItemData* ItemData = GetItemData(ItemToAdd.Name);
	if (ItemData == nullptr)
	{
		return false;
	}

	uint32 UniqueID = FMath::RandRange(1, 999999);
	Remaining = ItemToAdd.Quantity;
	
	// if the item stats are empty populate with defaults
	TArray<FItemStat> ItemStats;
	if (ItemData->Stats.Num() > 0 && ItemToAdd.Stats.Num() == 0)
	{
		ItemStats = ItemData->Stats;
	}
	else
	{
		ItemStats = ItemToAdd.Stats;
	}

	if (!FindAndAddToPopulatedSlot(ItemToAdd.Name, ItemData->StackSize, Remaining))
	{
		FindAndAddToEmptySlot(ItemToAdd.Name, ItemData->StackSize, ItemStats, Remaining);
	}

	return Remaining == 0;
}

bool UInventoryComponent::FindAndAddToPopulatedSlot(const FName& ItemName, const int32& ItemStackSize, int32& QuantityToAdd)
{
	for (FInventorySlot& Slot : Slots)
	{
		if (QuantityToAdd == 0)
		{
			break;
		}

		if (Slot.Item.Quantity == GetItemData(ItemName)->StackSize || Slot.Item.Name != ItemName)
		{
			continue;
		}

		if (Slot.Item.Quantity + QuantityToAdd > ItemStackSize)
		{
			QuantityToAdd -= ItemStackSize - Slot.Item.Quantity;
			FInventoryItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = ItemStackSize;
			Slot.Item = NewSlotItem;
		}
		else
		{
			FInventoryItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = QuantityToAdd + Slot.Item.Quantity;
			Slot.Item = NewSlotItem;
			QuantityToAdd = 0;
		}
	}
	return QuantityToAdd == 0;
}

bool UInventoryComponent::FindAndAddToEmptySlot(const FName& ItemName, const int32& ItemStackSize, const TArray<FItemStat>& Stats, int32& QuantityToAdd)
{
	for (FInventorySlot& Slot : Slots)
	{
		if (QuantityToAdd == 0)
		{
			break;
		}
		if (Slot.Item.Quantity > 0)
		{
			continue;
		}

		uint32 ItemUniqueID = FMath::RandRange(0, 99999);

		if (QuantityToAdd > ItemStackSize)
		{
			QuantityToAdd -= ItemStackSize;
			FInventoryItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = ItemStackSize;
			NewSlotItem.UniqueID = ItemUniqueID;
			Slot.Item = NewSlotItem;
		}
		else
		{
			FInventoryItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = QuantityToAdd;
			NewSlotItem.Stats = Stats;
			NewSlotItem.UniqueID = ItemUniqueID;
			Slot.Item = NewSlotItem;
			QuantityToAdd = 0;
		}
	}
	return QuantityToAdd == 0;
}

bool UInventoryComponent::RemoveItemFromSlots(const FName& ItemName, const int32& Quantity, int32& Remaining)
{
	Remaining = Quantity;

	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.Item.Name != ItemName)
		{
			continue;
		}

		if (Slot.Item.Quantity <= Remaining)
		{
			Remaining -= Slot.Item.Quantity;
			Slot.ClearItem();
			continue;
		}
		else
		{
			Slot.Item.Quantity -= Remaining;
			Remaining = 0;
		}
		
		if (Slot.Item.IsZero())
		{
			Slot.ClearItem();
		}

		if (Remaining == 0)
		{
			break;
		}
	}

	return Remaining == 0;
}

void UInventoryComponent::DragAll(const int32& FromSlotIndex)
{
	FInventorySlot& FromSlot = Slots[FromSlotIndex];

	if (FromSlot.IsEmpty())
	{
		return;
	}

	Manipulator->StartDragging(FromSlot.Item);

	Contents.RemoveItem(FromSlot.Item.Name, FromSlot.Item.Quantity);

	FromSlot.ClearItem();

	OnInventoryChange();
}

void UInventoryComponent::DragSplit(const int32& FromSlotIndex)
{
	FInventorySlot& FromSlot = Slots[FromSlotIndex];

	if (FromSlot.IsEmpty())
	{
		return;
	}

	// Just take all if there is only one item
	if (FromSlot.Item.Quantity == 1)
	{
		Manipulator->StartDragging(FromSlot.Item);
		FromSlot.ClearItem();

		return;
	}

	// Calculate the half
	int32 HalfQuantity = FromSlot.Item.Quantity / 2;

	// Start Dragging half
	FInventoryItem NewSelection = FromSlot.Item;
	NewSelection.Quantity -= HalfQuantity;


	Manipulator->StartDragging(NewSelection);
	Contents.RemoveItem(NewSelection.Name, NewSelection.Quantity);

	FInventoryItem NewSlotItem = FromSlot.Item;
	NewSlotItem.Quantity = HalfQuantity;
	FromSlot.Item = NewSlotItem;

	// Update the slot
	OnInventoryChange();
}

void UInventoryComponent::DropAll(const int32& ToSlotIndex)
{
	FInventorySlot& ToSlot = Slots[ToSlotIndex];
	FInventoryItem SelectedItem = Manipulator->GetSelectedItem();

	if (ToSlot.IsEmpty())
	{
		ToSlot.Item = SelectedItem;

		Contents.AddItem(SelectedItem.Name, SelectedItem.Quantity);

		Manipulator->StopDragging();
	}
	else if (ToSlot.SameItemNameAs(SelectedItem) && GetItemData(SelectedItem.Name)->StackSize != 1) // same item and the stack size for this item is not 1
	{
		if (WithinStackSize(ToSlot.Item, SelectedItem.Quantity)) // todo move to function
		{
			// Update Inventory
			ToSlot.Item.Quantity += SelectedItem.Quantity;
			Contents.AddItem(Manipulator->GetSelectedItem().Name, Manipulator->GetSelectedItem().Quantity);
			
			// Update Selection
			Manipulator->StopDragging();
		}
		else
		{
			int32 OldSlotItemQuantity = ToSlot.Item.Quantity;

			// Update Inventory
			int32 AmountAdded = GetItemData(ToSlot.Item.Name)->StackSize - ToSlot.Item.Quantity;
			ToSlot.Item.Quantity = GetItemData(SelectedItem.Name)->StackSize;
			Contents.AddItem(SelectedItem.Name, AmountAdded);
			
			// Update Selection
			SelectedItem.Quantity = (OldSlotItemQuantity + SelectedItem.Quantity) - GetItemData(SelectedItem.Name)->StackSize;
			Manipulator->StartDragging(SelectedItem);
		}
	}
	else
	{
		// Store the old item for swap
		FInventoryItem OldSlotItem = ToSlot.Item;

		// Update Inventory
		Contents.RemoveItem(OldSlotItem.Name, OldSlotItem.Quantity);
		Contents.AddItem(SelectedItem.Name, SelectedItem.Quantity);
		ToSlot.Item = SelectedItem;

		// Update Selection
		Manipulator->StartDragging(OldSlotItem);
	}

	if (Manipulator->GetSelectedItem().IsZero())
	{
		Manipulator->StopDragging();
	}

	OnInventoryChange();
}

void UInventoryComponent::DropSingle(const int32& ToSlotIndex)
{
	if (!Manipulator->IsDragging())
	{
		return;
	}

	FInventorySlot& ToSlot = Slots[ToSlotIndex];
	FInventoryItem SelectedItem = Manipulator->GetSelectedItem();

	if (ToSlot.IsEmpty())
	{
		// Update Inventory
		ToSlot.Item.Name = SelectedItem.Name;
		ToSlot.Item.Quantity = 1;
		Contents.AddItem(SelectedItem.Name, 1);

		// Update Selection
		FInventoryItem NewSelection = SelectedItem;
		NewSelection.Quantity -= 1;
		Manipulator->StartDragging(NewSelection);
	}
	else if (ToSlot.SameItemNameAs(SelectedItem) && WithinStackSize(ToSlot.Item, 1))
	{
		// Update Inventory
		ToSlot.Item.Quantity += 1;
		Contents.AddItem(SelectedItem.Name, 1);

		// Update Selection
		FInventoryItem NewSelection = SelectedItem;
		NewSelection.Quantity -= 1;
		Manipulator->StartDragging(NewSelection);
	}

	if (Manipulator->GetSelectedItem().Quantity <= 0)
	{
		Manipulator->StopDragging();
	}

	OnInventoryChange();
}

bool UInventoryComponent::WithinStackSize(const FInventoryItem& BaseItem, const int32& AmountToAdd)
{
	return (BaseItem.Quantity + AmountToAdd) <= GetItemData(BaseItem.Name)->StackSize;
}