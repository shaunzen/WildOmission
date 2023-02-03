// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryComponent.h"
#include "WildOmission/Widgets/PlayerUI/InventoryWidget.h"
#include "WildOmission/Actors/WorldItem.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// Make this component replicate
	SetIsReplicatedByDefault(true);
	
	// Find the item data table
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT("/Game/WildOmission/Blueprints/DataTables/DT_Items"));
	if (ItemDataTableObject.Succeeded())
	{
		ItemDataTable = ItemDataTableObject.Object;
	}

	SlotCount = 30;
	Dragging = false;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Contents);
	DOREPLIFETIME(UInventoryComponent, Slots);
	DOREPLIFETIME(UInventoryComponent, SelectedItem);
	DOREPLIFETIME(UInventoryComponent, Dragging);
}

void UInventoryComponent::Setup(UInventoryWidget* InInventoryWidget)
{
	// Create a link between the Widget and this component
	InventoryWidget = InInventoryWidget;
	InventoryWidget->Setup(this);

	// Initialize slots
	Slots.SetNum(SlotCount, false);
	for (int32 i = 0; i < SlotCount; ++i)
	{
		Slots[i].Index = i;
	}
}

//**************************************************************
// General Management
//**************************************************************

void UInventoryComponent::AddItem(const FName& ItemName, const int32& Quantity)
{
	int32 Remaining;
	int32 AmountAdded;
	bool AddSuccess = AddItemToSlots(ItemName, Quantity, Remaining);

	// Calculate how many were added
	AmountAdded = Quantity - Remaining;

	// Add item to item list
	Server_AddItem(ItemName, AmountAdded);

	if (AddSuccess == false)
	{
		SpawnWorldItem(ItemName, Remaining);
	}

	if (InventoryWidget == nullptr)
	{
		return;
	}
	InventoryWidget->Refresh();
}

void UInventoryComponent::RemoveItem(const FName& ItemName, const int32& Quantity, bool bSpawnInWorld)
{
	Server_RemoveItem(ItemName, Quantity);
	if (bSpawnInWorld == true)
	{
		Server_SpawnWorldItem(ItemName, Quantity);
	}
	else
	{
		int32 Remaining;
		bool RemoveSuccess = RemoveItemFromSlots(ItemName, Quantity, Remaining);
	}

	if (InventoryWidget == nullptr)
	{
		return;
	}
	InventoryWidget->Refresh();
}

void UInventoryComponent::SpawnWorldItem(const FName& ItemName, const int32& Quantity)
{
	// RPC on the server to spawn a world item of out specification
	Server_SpawnWorldItem(ItemName, Quantity);
}

//**************************************************************
// User Interaction
//**************************************************************

void UInventoryComponent::SlotInteraction(const int32& SlotIndex, bool Primary)
{
	// This is gross
	if (!Dragging)
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
}

void UInventoryComponent::DropSelectedItemInWorld(bool Single)
{
	if (!Dragging)
	{
		return;
	}
	if (Single == true)
	{
		RemoveItem(SelectedItem.Name, 1, true);

		SelectedItem.Quantity -= 1;
	}
	else
	{
		RemoveItem(SelectedItem.Name, SelectedItem.Quantity, true);
		SelectedItem.Clear();
	}

	if (SelectedItem.Quantity == 0)
	{
		SelectedItem.Clear();
		Dragging = false;
	}

	InventoryWidget->Refresh();
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

bool UInventoryComponent::IsDragging() const
{
	return Dragging;
}

FInventoryItem* UInventoryComponent::GetSelectedItem()
{
	return &SelectedItem;
}

UInventoryWidget* UInventoryComponent::GetWidget()
{
	return InventoryWidget;
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
	FInventorySlot& FromSlot = Slots[FromSlotIndex];

	if (FromSlot.IsEmpty())
	{
		return;
	}

	SelectedItem = FromSlot.Item;
	Dragging = true;

	FromSlot.ClearItem();
}

// TODO simplify this function
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
		SelectedItem = FromSlot.Item;
		Dragging = true;

		//Take all
		FromSlot.ClearItem();

		return;
	}

	// Calculate the half
	int32 HalfQuantity = FromSlot.Item.Quantity / 2;

	// Start Dragging half
	FInventoryItem NewSelection = FromSlot.Item;
	NewSelection.Quantity = HalfQuantity;
	SelectedItem = NewSelection;
	Dragging = true;

	// Update the slot
	FInventoryItem NewSlotItem = FromSlot.Item;
	NewSlotItem.Quantity -= HalfQuantity;
	FromSlot.Item = NewSlotItem;
}

void UInventoryComponent::DropAll(const int32& ToSlotIndex)
{
	if (!Dragging)
	{
		return;
	}

	FInventorySlot& ToSlot = Slots[ToSlotIndex];

	if (ToSlot.IsEmpty())
	{
		ToSlot.Item = SelectedItem;
	
		SelectedItem.Clear();
		Dragging = false;
	}
	else if (ToSlot.CompareItem(SelectedItem))
	{
		if ((ToSlot.Item.Quantity + SelectedItem.Quantity) <= GetItemData(SelectedItem.Name)->StackSize)
		{
			ToSlot.Item.Quantity += SelectedItem.Quantity;
			
			SelectedItem.Clear();
			Dragging = false;
		}
		else
		{
			SelectedItem.Quantity = (ToSlot.Item.Quantity + SelectedItem.Quantity) - GetItemData(SelectedItem.Name)->StackSize;
			
			ToSlot.Item.Quantity = GetItemData(SelectedItem.Name)->StackSize;
		}
	}
	else
	{
		// Swap
		FInventoryItem OldSlotItem = ToSlot.Item;

		ToSlot.Item = SelectedItem;
		SelectedItem = OldSlotItem;
	}

	if (SelectedItem.IsZero())
	{
		SelectedItem.Clear();
		Dragging = false;
	}
}

void UInventoryComponent::DropSingle(const int32& ToSlotIndex)
{
	if (!Dragging)
	{
		return;
	}

	FInventorySlot& ToSlot = Slots[ToSlotIndex];

	if (ToSlot.IsEmpty())
	{
		// Set slot to one
		ToSlot.Item.Name = SelectedItem.Name;
		ToSlot.Item.Quantity = 1;

		// Remove one from dragging
		SelectedItem.Quantity -= 1;
	}
	else if (ToSlot.CompareItem(SelectedItem) && (ToSlot.Item.Quantity + 1) <= GetItemData(SelectedItem.Name)->StackSize)
	{
		// Add one to slot
		ToSlot.Item.Quantity += 1;

		// Remove one from selection
		SelectedItem.Quantity -= 1;
	}

	if (SelectedItem.Quantity <= 0)
	{
		SelectedItem.Clear();
		Dragging = false;
	}
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

//**************************************************************
// RPC
//**************************************************************

void UInventoryComponent::Server_AddItem_Implementation(const FName& ItemName, const int32& Quantity)
{
	// If the item already exists in the contents map
	Contents.AddItem(ItemName, Quantity);
}

bool UInventoryComponent::Server_RemoveItem_Validate(const FName& ItemName, const int32& Quantity)
{
	// Only valid if the player has the item they are removing
	return Contents.HasItem(ItemName);
}

void UInventoryComponent::Server_RemoveItem_Implementation(const FName& ItemName, const int32& Quantity)
{
	// Find the item
	Contents.RemoveItem(ItemName, Quantity);
}

void UInventoryComponent::Server_SpawnWorldItem_Implementation(const FName& ItemName, const int32& Quantity)
{
	// Get the data for this item
	FItem* ItemData = GetItemData(ItemName);
	// Spawn a world item actor
	AWorldItem* WorldItem = GetWorld()->SpawnActor<AWorldItem>();
	if (ItemData == nullptr || WorldItem == nullptr)
	{
		return;
	}

	// Update world items properties
	WorldItem->Client_SetItemProperties(ItemName, Quantity, ItemData->Mesh, GetOwner()->GetActorLocation());
}