// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryComponent.h"
#include "WildOmission/Widgets/PlayerUI/InventoryWidget.h"
#include "WildOmission/Actors/WorldItem.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
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
}

void UInventoryComponent::Setup(UInventoryWidget* InInventoryWidget)
{
	// Create a link between the Widget and this component
	InventoryWidget = InInventoryWidget;
	InventoryWidget->Setup(this);

	Slots.SetNum(SlotCount, false);
}

void UInventoryComponent::AddItem(const FName& ItemName, const int32& Quantity)
{
	int32 Remaining;
	int32 AmountAdded;
	bool AddSuccess = AddItemToSlots(ItemName, Quantity, Remaining);

	// Calculate how many were added
	AmountAdded = Quantity - Remaining;

	// Add item to item list
	Server_AddItem(ItemName, AmountAdded);

	InventoryWidget->Refresh();

	if (AddSuccess == false)
	{
		SpawnWorldItem(ItemName, Remaining);
	}
}

void UInventoryComponent::Server_AddItem_Implementation(const FName& ItemName, const int32& Quantity)
{
	// If the item already exists in the contents map
	Contents.AddItem(ItemName, Quantity);
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
	InventoryWidget->Refresh();
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

void UInventoryComponent::SwapItem()
{
	// TODO Swap item
}

void UInventoryComponent::SpawnWorldItem(const FName& ItemName, const int32& Quantity)
{
	// RPC on the server to spawn a world item of out specification
	Server_SpawnWorldItem(ItemName, Quantity);
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

FItem* UInventoryComponent::GetItemData(const FName& ItemName)
{
	if (ItemDataTable == nullptr)
	{
		return nullptr;
	}
	static const FString ContextString(TEXT("Item Data Context"));
	return ItemDataTable->FindRow<FItem>(ItemName, ContextString, true);
}

FInventoryContents* UInventoryComponent::GetContents()
{
	return &Contents;
}

UInventoryWidget* UInventoryComponent::GetWidget()
{
	return InventoryWidget;
}

////////////////////////////////////////////////////////////////////////
//	Slot Stuff
/////////////////////////////////////////////////////////////////////////
bool UInventoryComponent::AddItemToSlots(const FName& ItemName, const int32& Quantity, int32& Remaining)
{
	int32 QuantityToAdd = Quantity;
	FItem* ItemData = GetItemData(ItemName);
	if (ItemData == nullptr)
	{
		return false;
	}

	if (!AddItemToPopulatedSlot(ItemName, ItemData, QuantityToAdd))
	{
		AddItemToEmptySlot(ItemName, ItemData, QuantityToAdd);
	}

	Remaining = QuantityToAdd;

	return QuantityToAdd == 0;
}

bool UInventoryComponent::AddItemToPopulatedSlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd)
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

bool UInventoryComponent::AddItemToEmptySlot(const FName& ItemName, FItem* ItemData, int32& QuantityToAdd)
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

void UInventoryComponent::SlotInteraction(const int32& SlotIndex, bool Primary)
{
	if (!Dragging)
	{
		Drag(SlotIndex, !Primary);
	}
	else
	{
		Drop(SlotIndex, !Primary);
	}
}

void UInventoryComponent::Drag(const int32& FromSlotIndex, bool bSplit)
{
	// Don't Drag item from this slot if there is no item
	if (Slots[FromSlotIndex].Item.Quantity == 0)
	{
		return;
	}

	if (bSplit == false)
	{
		//Take all
		// Start Dragging item in from index
		SelectedItem = Slots[FromSlotIndex].Item;
		Dragging = true;

		//Take all
		Slots[FromSlotIndex].ClearItem();
	}
	else
	{
		// Just take all if there is only one item
		if (Slots[FromSlotIndex].Item.Quantity == 1)
		{
			// Take all		
			// Start Dragging item in from index
			SelectedItem = Slots[FromSlotIndex].Item;
			Dragging = true;

			//Take all
			Slots[FromSlotIndex].ClearItem();

			return;
		}
		
		// Calculate the half
		int32 HalfQuantity = Slots[FromSlotIndex].Item.Quantity / 2;

		// Start Dragging half
		FInventoryItem NewSelection = Slots[FromSlotIndex].Item;
		NewSelection.Quantity = HalfQuantity;
		SelectedItem = NewSelection;
		Dragging = true;

		// Update the slot
		FInventoryItem NewSlotItem = Slots[FromSlotIndex].Item;
		NewSlotItem.Quantity -= HalfQuantity;
		Slots[FromSlotIndex].Item = NewSlotItem;
	}
}

void UInventoryComponent::Drop(const int32& ToSlotIndex, bool bSingle)
{
	if (Dragging == false)
	{
		return;
	}

	FInventorySlot& ToSlot = Slots[ToSlotIndex];

	if (bSingle == false)
	{
		//TODO is empty function on inventory item
		// If the slot we are droping to is empty just give it all
		if (ToSlot.Item.Quantity == 0)
		{
			// Set slot item to selected
			ToSlot.Item = SelectedItem;
			// Clear selected
			SelectedItem.Clear();
			// stop dragging
			Dragging = false;
		}
		else if (ToSlot.Item.Name == SelectedItem.Name)
		{
			// if the amount is within stack size
			if ((ToSlot.Item.Quantity + SelectedItem.Quantity) <= GetItemData(SelectedItem.Name)->StackSize)
			{
				// Give it all
				FInventoryItem NewSlotItem = ToSlot.Item;
				NewSlotItem.Quantity += SelectedItem.Quantity;
				ToSlot.Item = NewSlotItem;
				SelectedItem.Clear();
				Dragging = false;
			}
			else
			{
				FInventoryItem NewSelection = SelectedItem;
				NewSelection.Quantity = (ToSlot.Item.Quantity + SelectedItem.Quantity) - GetItemData(SelectedItem.Name)->StackSize;
				SelectedItem = NewSelection;
				
				FInventoryItem NewSlotItem = ToSlot.Item;
				NewSlotItem.Quantity = GetItemData(SelectedItem.Name)->StackSize;
				ToSlot.Item = NewSlotItem;
			}
		}
		else
		{
			// Swap slots
			FInventoryItem OldSlotItem = ToSlot.Item;

			ToSlot.Item = SelectedItem;
			SelectedItem = OldSlotItem;
		}
	}
	else
	{
		if (ToSlot.Item.Quantity == 0)
		{
			// Set slot to one
			ToSlot.Item.Name = SelectedItem.Name;
			ToSlot.Item.Quantity = 1;

			// Remove one from dragging
			SelectedItem.Quantity -= 1;
		}
		else if (ToSlot.Item.Name == SelectedItem.Name && (ToSlot.Item.Quantity + 1) <= GetItemData(SelectedItem.Name)->StackSize)
		{
			// Add one to slot
			ToSlot.Item.Quantity += 1;

			// Remove one from selection
			SelectedItem.Quantity -= 1;
		}
	}
	
	if (SelectedItem.Quantity <= 0)
	{
		SelectedItem.Clear();
		Dragging = false;
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

TArray<FInventorySlot>& UInventoryComponent::GetSlots()
{
	return Slots;
}

FInventoryItem* UInventoryComponent::GetSelectedItem()
{
	return &SelectedItem;
}

bool UInventoryComponent::IsDragging() const
{
	return Dragging;
}

// Save/Load
FWildOmissionInventorySave UInventoryComponent::Save()
{
	FWildOmissionInventorySave Save;

	// get and save our items
	Save.Items = Contents.Contents;
	// get and save slots

	return Save;
}

void UInventoryComponent::Load(const FWildOmissionInventorySave& InInventorySave)
{
	Contents.Contents = InInventorySave.Items;
}
