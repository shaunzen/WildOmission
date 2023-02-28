// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryComponent.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// Make this component replicate
	SetIsReplicatedByDefault(true);
	
	// Find the item data table
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

//**************************************************************
// General Management
//**************************************************************

void UInventoryComponent::AddItem(const FName& ItemName, const int32& Quantity)
{
	Server_AddItem(ItemName, Quantity);
	RefreshUI();
}

void UInventoryComponent::RemoveItem(const FName& ItemName, const int32& Quantity, bool bSpawnInWorld)
{
	Server_RemoveItem(ItemName, Quantity);
	RefreshUI();
}

//**************************************************************
// User Interaction
//**************************************************************

void UInventoryComponent::SlotInteraction(const int32& SlotIndex, bool Primary)
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

//**************************************************************
// Getters
//**************************************************************

FInventoryContents* UInventoryComponent::GetContents()
{
	return &Contents;
}

TArray<FInventorySlot>& UInventoryComponent::GetSlots()
{
	return Slots;
}

FItem* UInventoryComponent::GetItemData(const FName& ItemName)
{
	if (ItemDataTable == nullptr)
	{
		return nullptr;
	}
	static const FString ContextString(TEXT("Item Data Context"));
	return ItemDataTable->FindRow<FItem>(ItemName, ContextString, true);
}

UInventoryManipulatorComponent* UInventoryComponent::GetManipulator()
{
	return Manipulator;
}

//**************************************************************
// Save Load
//**************************************************************

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

//**************************************************************
// Slot Functions
//**************************************************************

bool UInventoryComponent::AddItemToSlots(const FName& ItemName, const int32& Quantity, int32& Remaining)
{
	int32 QuantityToAdd = Quantity;
	FItem* ItemData = GetItemData(ItemName);
	if (ItemData == nullptr)
	{
		return false;
	}

	if (!FindAndAddToPopulatedSlot(ItemName, ItemData, QuantityToAdd))
	{
		FindAndAddToEmptySlot(ItemName, ItemData, QuantityToAdd);
	}

	Remaining = QuantityToAdd;

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

		if (Remaining == 0)
		{
			break;
		}
	}

	return Remaining == 0;
}

void UInventoryComponent::DragAll(const int32& FromSlotIndex)
{
	Server_DragAll(FromSlotIndex);
}

void UInventoryComponent::DragSplit(const int32& FromSlotIndex)
{
	Server_DragSplit(FromSlotIndex);
}

void UInventoryComponent::DropAll(const int32& ToSlotIndex)
{
	Server_DropAll(ToSlotIndex);
}

void UInventoryComponent::DropSingle(const int32& ToSlotIndex)
{
	Server_DropSingle(ToSlotIndex);
}

bool UInventoryComponent::FindAndAddToPopulatedSlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd)
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

		if (Slot.Item.Quantity + QuantityToAdd > ItemData->StackSize)
		{
			QuantityToAdd -= ItemData->StackSize - Slot.Item.Quantity;
			FInventoryItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = ItemData->StackSize;
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

bool UInventoryComponent::FindAndAddToEmptySlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd)
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

		if (QuantityToAdd > ItemData->StackSize)
		{
			QuantityToAdd -= ItemData->StackSize;
			FInventoryItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = ItemData->StackSize;
			Slot.Item = NewSlotItem;
		}
		else
		{
			FInventoryItem NewSlotItem;
			NewSlotItem.Name = ItemName;
			NewSlotItem.Quantity = QuantityToAdd;
			Slot.Item = NewSlotItem;
			QuantityToAdd = 0;
		}
	}
	return QuantityToAdd == 0;
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

void UInventoryComponent::OnInventoryChange()
{
	RefreshUI();
}

//**************************************************************
// RPC
//**************************************************************
void UInventoryComponent::Server_AddItemToContents_Implementation(const FName& ItemName, const int32& Quantity)
{
	// Add item to item list
	Contents.AddItem(ItemName, Quantity);
}

void UInventoryComponent::Server_AddItem_Implementation(const FName& ItemName, const int32& Quantity)
{
	int32 Remaining;
	int32 AmountAdded;
	bool AddSuccess = AddItemToSlots(ItemName, Quantity, Remaining);

	// Calculate how many were added
	AmountAdded = Quantity - Remaining;

	// Add item to item list
	Contents.AddItem(ItemName, AmountAdded);
	
	if (AddSuccess == false)
	{
		Manipulator->SpawnWorldItem(ItemName, Remaining);
	}

	OnInventoryChange();
}

bool UInventoryComponent::Server_RemoveItemFromContents_Validate(const FName& ItemName, const int32& Quantity)
{
	return Contents.HasItem(ItemName);
}

void UInventoryComponent::Server_RemoveItemFromContents_Implementation(const FName& ItemName, const int32& Quantity)
{
	Contents.RemoveItem(ItemName, Quantity);
}

bool UInventoryComponent::Server_RemoveItem_Validate(const FName& ItemName, const int32& Quantity)
{
	// Only valid if the player has the item they are removing
	return Contents.HasItem(ItemName);
}

void UInventoryComponent::Server_RemoveItem_Implementation(const FName& ItemName, const int32& Quantity)
{
	// Remove from contents
	Contents.RemoveItem(ItemName, Quantity);
	
	// Remove from slots
	int32 Remaining;
	bool RemoveSuccess = RemoveItemFromSlots(ItemName, Quantity, Remaining);
	
	// Call inventory change
	OnInventoryChange();
}

void UInventoryComponent::Server_DragAll_Implementation(const int32& FromSlotIndex)
{
	FInventorySlot& FromSlot = Slots[FromSlotIndex];

	if (FromSlot.IsEmpty())
	{
		return;
	}

	Manipulator->StartDragging(FromSlot.Item);

	Server_RemoveItemFromContents(FromSlot.Item.Name, FromSlot.Item.Quantity);
	
	FromSlot.ClearItem();


	OnInventoryChange();
}

// TODO simplify this function
void UInventoryComponent::Server_DragSplit_Implementation(const int32& FromSlotIndex)
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
	
	Server_RemoveItemFromContents(NewSelection.Name, NewSelection.Quantity);

	// Update the slot
	FInventoryItem NewSlotItem = FromSlot.Item;
	NewSlotItem.Quantity -= HalfQuantity;
	FromSlot.Item = NewSlotItem;

	OnInventoryChange();
}

void UInventoryComponent::Server_DropAll_Implementation(const int32& ToSlotIndex)
{
	FInventorySlot& ToSlot = Slots[ToSlotIndex];

	if (ToSlot.IsEmpty())
	{
		ToSlot.Item = Manipulator->GetSelectedItem();
		
		Server_AddItemToContents(Manipulator->GetSelectedItem().Name, Manipulator->GetSelectedItem().Quantity);

		Manipulator->StopDragging();
	}
	else if (ToSlot.CompareItem(Manipulator->GetSelectedItem()))
	{
		if ((ToSlot.Item.Quantity + Manipulator->GetSelectedItem().Quantity) <= GetItemData(Manipulator->GetSelectedItem().Name)->StackSize)
		{
			ToSlot.Item.Quantity += Manipulator->GetSelectedItem().Quantity;

			Server_AddItemToContents(Manipulator->GetSelectedItem().Name, Manipulator->GetSelectedItem().Quantity);

			Manipulator->StopDragging();
		}
		else
		{
			FInventoryItem NewSelection = ToSlot.Item;
			
			NewSelection.Quantity = (ToSlot.Item.Quantity + Manipulator->GetSelectedItem().Quantity) - GetItemData(Manipulator->GetSelectedItem().Name)->StackSize;

			// figure out how much is actually added
			int32 AmountAdded = GetItemData(ToSlot.Item.Name)->StackSize - ToSlot.Item.Quantity;

			Server_AddItemToContents(NewSelection.Name, AmountAdded);

			Manipulator->StartDragging(NewSelection);
			
			ToSlot.Item.Quantity = GetItemData(ToSlot.Item.Name)->StackSize;
		}
	}
	else
	{
		// Swap
		FInventoryItem OldSlotItem = ToSlot.Item;

		Server_RemoveItemFromContents(OldSlotItem.Name, OldSlotItem.Quantity);
		Server_AddItemToContents(Manipulator->GetSelectedItem().Name, Manipulator->GetSelectedItem().Quantity);

		ToSlot.Item = Manipulator->GetSelectedItem();

		Manipulator->StartDragging(OldSlotItem);
	}
	
	if (Manipulator->GetSelectedItem().IsZero())
	{
		Manipulator->StopDragging();
	}

	OnInventoryChange();
}

void UInventoryComponent::Server_DropSingle_Implementation(const int32& ToSlotIndex)
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

		Server_AddItemToContents(Manipulator->GetSelectedItem().Name, 1);

		FInventoryItem NewSelection = Manipulator->GetSelectedItem();
		NewSelection.Quantity -= 1;
		
		// Remove one from dragging
		Manipulator->StartDragging(NewSelection);
	}
	else if (ToSlot.CompareItem(Manipulator->GetSelectedItem()) && (ToSlot.Item.Quantity + 1) <= GetItemData(Manipulator->GetSelectedItem().Name)->StackSize)
	{
		// Add one to slot
		ToSlot.Item.Quantity += 1;

		Server_AddItemToContents(Manipulator->GetSelectedItem().Name, 1);

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