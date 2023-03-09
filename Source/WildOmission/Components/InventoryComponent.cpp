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

	ManipulatorHUD->RefreshInventoryStates();
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

	// todo temp
	UE_LOG(LogTemp, Warning, TEXT("FromSlot Item Unique ID: %i"), FromSlot.Item.UniqueID);
	for (const FItemStat& Stat : FromSlot.Item.Stats)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dragging Item with stat: %s: %i"), *Stat.Name.ToString(), Stat.Value);
	}

	//UE_LOG(LogTemp, Warning, TEXT("FromSlot Item Durability: %d"), FromSlot.Item.Durability);

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
		// Take all		
		// Start Dragging item in from index
		Manipulator->StartDragging(FromSlot.Item);

		//Take all
		FromSlot.ClearItem();

		return;
	}

	// Calculate the half
	int32 HalfQuantity = FromSlot.Item.Quantity / 2;

	// Start Dragging half
	FInventoryItem NewSelection = FromSlot.Item;
	NewSelection.Quantity = HalfQuantity;

	Manipulator->StartDragging(NewSelection);

	Contents.RemoveItem(NewSelection.Name, NewSelection.Quantity);

	// Update the slot
	FInventoryItem NewSlotItem = FromSlot.Item;
	NewSlotItem.Quantity -= HalfQuantity;
	FromSlot.Item = NewSlotItem;

	OnInventoryChange();

}

void UInventoryComponent::DropAll(const int32& ToSlotIndex)
{
	FInventorySlot& ToSlot = Slots[ToSlotIndex];

	if (ToSlot.IsEmpty())
	{
		ToSlot.Item = Manipulator->GetSelectedItem();

		Contents.AddItem(Manipulator->GetSelectedItem().Name, Manipulator->GetSelectedItem().Quantity);

		Manipulator->StopDragging();
	}
	else if (ToSlot.SameItemNameAs(Manipulator->GetSelectedItem()) && GetItemData(Manipulator->GetSelectedItem().Name)->StackSize != 1) // and not something unique?
	{
		if ((ToSlot.Item.Quantity + Manipulator->GetSelectedItem().Quantity) <= GetItemData(Manipulator->GetSelectedItem().Name)->StackSize)
		{
			ToSlot.Item.Quantity += Manipulator->GetSelectedItem().Quantity;

			Contents.AddItem(Manipulator->GetSelectedItem().Name, Manipulator->GetSelectedItem().Quantity);
			
			Manipulator->StopDragging();
		}
		else
		{
			FInventoryItem NewSelection = ToSlot.Item;

			NewSelection.Quantity = (ToSlot.Item.Quantity + Manipulator->GetSelectedItem().Quantity) - GetItemData(Manipulator->GetSelectedItem().Name)->StackSize;

			// figure out how much is actually added
			int32 AmountAdded = GetItemData(ToSlot.Item.Name)->StackSize - ToSlot.Item.Quantity;

			Contents.AddItem(NewSelection.Name, AmountAdded);
			
			Manipulator->StartDragging(NewSelection);

			ToSlot.Item.Quantity = GetItemData(ToSlot.Item.Name)->StackSize;
		}
	}
	else
	{
		// Swap
		FInventoryItem OldSlotItem = ToSlot.Item;

		Contents.RemoveItem(OldSlotItem.Name, OldSlotItem.Quantity);
		Contents.AddItem(Manipulator->GetSelectedItem().Name, Manipulator->GetSelectedItem().Quantity);

		ToSlot.Item = Manipulator->GetSelectedItem();

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

	if (ToSlot.IsEmpty())
	{
		// Set slot to one
		ToSlot.Item.Name = Manipulator->GetSelectedItem().Name;
		ToSlot.Item.Quantity = 1;

		Contents.AddItem(Manipulator->GetSelectedItem().Name, 1);

		FInventoryItem NewSelection = Manipulator->GetSelectedItem();
		NewSelection.Quantity -= 1;

		// Remove one from dragging
		Manipulator->StartDragging(NewSelection);
	}
	else if (ToSlot.SameItemNameAs(Manipulator->GetSelectedItem()) && (ToSlot.Item.Quantity + 1) <= GetItemData(Manipulator->GetSelectedItem().Name)->StackSize)
	{
		// Add one to slot
		ToSlot.Item.Quantity += 1;

		Contents.AddItem(Manipulator->GetSelectedItem().Name, 1);

		// Remove one from selection
		FInventoryItem NewSelection = Manipulator->GetSelectedItem();
		NewSelection.Quantity -= 1;
	}

	if (Manipulator->GetSelectedItem().Quantity <= 0)
	{
		Manipulator->StopDragging();
	}

	OnInventoryChange();
}