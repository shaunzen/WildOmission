// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryComponent.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "WildOmission/Core/WildOmissionStatics.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);

	SlotCount = 24;
	LoadedFromSave = false;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, ServerState);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->HasAuthority() || ServerState.Slots.Num() == SlotCount)
	{
		return;
	}

	// Initialize slots
	ServerState.Slots.SetNum(SlotCount, false);
	for (int32 i = 0; i < SlotCount; ++i)
	{
		ServerState.Slots[i].Index = i;
	}

	LoadedFromSave = false;
}

void UInventoryComponent::AddItem(const FInventoryItem& ItemToAdd, AActor* ActorToSpawnDropedItems)
{
	// The amount of items we were unable to add
	int32 Remaining;
	
	// The amount of items we successfully added
	int32 AmountAdded;
	
	bool AddSuccess = AddItemToSlots(ItemToAdd, Remaining);

	// Calculate how many were added and add that amount to our contents
	AmountAdded = ItemToAdd.Quantity - Remaining;
	ServerState.Contents.AddItem(ItemToAdd.Name, AmountAdded);

	if (AddSuccess == false)
	{
		FInventoryItem DroppedItem = ItemToAdd;
		DroppedItem.Quantity = Remaining;

		AActor* DropperActor = nullptr;
		if (ActorToSpawnDropedItems == nullptr)
		{
			DropperActor = GetOwner();
		}
		else
		{
			DropperActor = ActorToSpawnDropedItems;
		}
		UWildOmissionStatics::SpawnWorldItem(GetWorld(), DroppedItem, DropperActor);
	}

	OnRep_ServerState();
}

void UInventoryComponent::RemoveItem(const FInventoryItem& ItemToRemove)
{
	int32 Remaining;
	int32 AmountSuccessfullyRemoved;
	
	RemoveItemFromSlots(ItemToRemove.Name, ItemToRemove.Quantity, Remaining);
	AmountSuccessfullyRemoved = ItemToRemove.Quantity - Remaining;

	// Remove from contents
	ServerState.Contents.RemoveItem(ItemToRemove.Name, AmountSuccessfullyRemoved);

	// Call inventory change
	OnRep_ServerState();
}

void UInventoryComponent::SlotInteraction(const int32& SlotIndex, UInventoryManipulatorComponent* Manipulator, bool Primary)
{
	// Create an interaction
	FInventorySlotInteraction CurrentInteraction;
	CurrentInteraction.SlotIndex = SlotIndex;
	CurrentInteraction.Manipulator = Manipulator;
	CurrentInteraction.Primary = Primary;
	CurrentInteraction.Time = GetWorld()->TimeSeconds;

	// Send it to the server
	Server_SlotInteraction(CurrentInteraction);

	// Add it to our unacknowlaged interactions
	UnacknowalgedInteractions.Add(CurrentInteraction);

	if (GetOwner()->HasAuthority())
	{
		return;
	}

	// This is gross
	if (!Manipulator->IsDragging())
	{
		if (Primary)
		{
			DragAll(SlotIndex, Manipulator, Slots, Contents);
		}
		else
		{
			DragSplit(SlotIndex, Manipulator, Slots, Contents);
		}
	}
	else
	{
		if (Primary)
		{
			DropAll(SlotIndex, Manipulator, Slots, Contents);
		}
		else
		{
			DropSingle(SlotIndex, Manipulator, Slots, Contents);
		}
	}

	BroadcastInventoryUpdate();
}

void UInventoryComponent::OnLoadComplete_Implementation()
{
	ServerState.Contents = Contents;
	ServerState.Slots = Slots;
}

void UInventoryComponent::Server_SlotInteraction_Implementation(FInventorySlotInteraction Interaction)
{
	// Update our server state
	ServerState.LastInteraction = Interaction;

	if (!Interaction.Manipulator->IsDragging())
	{
		if (Interaction.Primary)
		{
			DragAll(Interaction.SlotIndex, Interaction.Manipulator, ServerState.Slots, ServerState.Contents);
		}
		else
		{
			DragSplit(Interaction.SlotIndex, Interaction.Manipulator, ServerState.Slots, ServerState.Contents);
		}
	}
	else
	{
		if (Interaction.Primary)
		{
			DropAll(Interaction.SlotIndex, Interaction.Manipulator, ServerState.Slots, ServerState.Contents);
		}
		else
		{
			DropSingle(Interaction.SlotIndex, Interaction.Manipulator, ServerState.Slots, ServerState.Contents);
		}
	}

	OnRep_ServerState();
}

FInventoryItem* UInventoryComponent::FindItemWithUniqueID(const uint32& UniqueID)
{
	FInventoryItem* FoundItem = nullptr;

	for (FInventorySlot& Slot : ServerState.Slots)
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

	for (FInventorySlot& Slot : ServerState.Slots)
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

uint8 UInventoryComponent::GetSlotCount() const
{
	return SlotCount;
}

TArray<FInventorySlot>& UInventoryComponent::GetSlots()
{
	return Slots;
}

FWildOmissionInventorySave UInventoryComponent::Save()
{
	FWildOmissionInventorySave Save;

	// Save Contents List
	Save.Items = ServerState.Contents.Contents;
	
	// Save Slots
	Save.Slots = ServerState.Slots;

	return Save;
}

void UInventoryComponent::Load(const FWildOmissionInventorySave& InInventorySave)
{
	ServerState.Contents.Contents = InInventorySave.Items;
	ServerState.Slots = InInventorySave.Slots;
	LoadedFromSave = true;

	OnRep_ServerState();
}

void UInventoryComponent::OnRep_ServerState()
{
	ClearAcknowlagedInteractions(ServerState.LastInteraction);
	
	if (GetOwner()->HasAuthority() || UnacknowalgedInteractions.Num() == 0 || ServerState.LastInteraction.Time == 0)
	{
		Slots = ServerState.Slots;
		Contents = ServerState.Contents;
	}

	BroadcastInventoryUpdate();
}

void UInventoryComponent::ClearAcknowlagedInteractions(const FInventorySlotInteraction& LastInteraction)
{
	TArray<FInventorySlotInteraction> NewInteractionsList;

	for (const FInventorySlotInteraction& Interaction : UnacknowalgedInteractions)
	{
		if (Interaction.Time > LastInteraction.Time)
		{
			NewInteractionsList.Add(Interaction);
		}
	}

	UnacknowalgedInteractions = NewInteractionsList;
}

void UInventoryComponent::BroadcastInventoryUpdate()
{
	if (!OnUpdate.IsBound())
	{
		return;
	}

	OnUpdate.Broadcast();
}

bool UInventoryComponent::AddItemToSlots(const FInventoryItem& ItemToAdd, int32& Remaining)
{
	FItemData* ItemData = UWildOmissionStatics::GetItemData(ItemToAdd.Name);
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
	for (FInventorySlot& Slot : ServerState.Slots)
	{
		if (QuantityToAdd == 0)
		{
			break;
		}

		if (Slot.Item.Quantity == UWildOmissionStatics::GetItemData(ItemName)->StackSize || Slot.Item.Name != ItemName)
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
	for (FInventorySlot& Slot : ServerState.Slots)
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

	for (FInventorySlot& Slot : ServerState.Slots)
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

void UInventoryComponent::DragAll(const int32& FromSlotIndex, UInventoryManipulatorComponent* Manipulator, TArray<FInventorySlot>& SlotsToModify, FInventoryContents& ContentsToModify)
{
	FInventorySlot& FromSlot = SlotsToModify[FromSlotIndex];

	if (FromSlot.IsEmpty())
	{
		return;
	}

	Manipulator->StartDragging(FromSlot.Item);

	ContentsToModify.RemoveItem(FromSlot.Item.Name, FromSlot.Item.Quantity);

	FromSlot.ClearItem();
}

void UInventoryComponent::DragSplit(const int32& FromSlotIndex, UInventoryManipulatorComponent* Manipulator, TArray<FInventorySlot>& SlotsToModify, FInventoryContents& ContentsToModify)
{
	FInventorySlot& FromSlot = SlotsToModify[FromSlotIndex];

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
	ContentsToModify.RemoveItem(NewSelection.Name, NewSelection.Quantity);

	FInventoryItem NewSlotItem = FromSlot.Item;
	NewSlotItem.Quantity = HalfQuantity;
	FromSlot.Item = NewSlotItem;
}

void UInventoryComponent::DropAll(const int32& ToSlotIndex, UInventoryManipulatorComponent* Manipulator, TArray<FInventorySlot>& SlotsToModify, FInventoryContents& ContentsToModify)
{
	FInventorySlot& ToSlot = SlotsToModify[ToSlotIndex];
	FInventoryItem SelectedItem = Manipulator->GetSelectedItem();

	if (ToSlot.IsEmpty())
	{
		ToSlot.Item = SelectedItem;

		ContentsToModify.AddItem(SelectedItem.Name, SelectedItem.Quantity);

		Manipulator->StopDragging();
	}
	else if (ToSlot.SameItemNameAs(SelectedItem) && UWildOmissionStatics::UWildOmissionStatics::GetItemData(SelectedItem.Name)->StackSize != 1) // same item and the stack size for this item is not 1
	{
		if (WithinStackSize(ToSlot.Item, SelectedItem.Quantity)) // todo move to function
		{
			// Update Inventory
			ToSlot.Item.Quantity += SelectedItem.Quantity;
			ContentsToModify.AddItem(Manipulator->GetSelectedItem().Name, Manipulator->GetSelectedItem().Quantity);
			
			// Update Selection
			Manipulator->StopDragging();
		}
		else
		{
			int32 OldSlotItemQuantity = ToSlot.Item.Quantity;

			// Update Inventory
			int32 AmountAdded = UWildOmissionStatics::GetItemData(ToSlot.Item.Name)->StackSize - ToSlot.Item.Quantity;
			ToSlot.Item.Quantity = UWildOmissionStatics::GetItemData(SelectedItem.Name)->StackSize;
			ContentsToModify.AddItem(SelectedItem.Name, AmountAdded);
			
			// Update Selection
			SelectedItem.Quantity = (OldSlotItemQuantity + SelectedItem.Quantity) - UWildOmissionStatics::GetItemData(SelectedItem.Name)->StackSize;
			Manipulator->StartDragging(SelectedItem);
		}
	}
	else
	{
		// Store the old item for swap
		FInventoryItem OldSlotItem = ToSlot.Item;

		// Update Inventory
		ContentsToModify.RemoveItem(OldSlotItem.Name, OldSlotItem.Quantity);
		ContentsToModify.AddItem(SelectedItem.Name, SelectedItem.Quantity);
		ToSlot.Item = SelectedItem;

		// Update Selection
		Manipulator->StartDragging(OldSlotItem);
	}

	if (Manipulator->GetSelectedItem().IsZero())
	{
		Manipulator->StopDragging();
	}
}

void UInventoryComponent::DropSingle(const int32& ToSlotIndex, UInventoryManipulatorComponent* Manipulator, TArray<FInventorySlot>& SlotsToModify, FInventoryContents& ContentsToModify)
{
	if (!Manipulator->IsDragging())
	{
		return;
	}

	FInventorySlot& ToSlot = SlotsToModify[ToSlotIndex];
	FInventoryItem SelectedItem = Manipulator->GetSelectedItem();

	if (ToSlot.IsEmpty())
	{
		// Update Inventory
		ToSlot.Item.Name = SelectedItem.Name;
		ToSlot.Item.Quantity = 1;
		ContentsToModify.AddItem(SelectedItem.Name, 1);

		// Update Selection
		FInventoryItem NewSelection = SelectedItem;
		NewSelection.Quantity -= 1;
		Manipulator->StartDragging(NewSelection);
	}
	else if (ToSlot.SameItemNameAs(SelectedItem) && WithinStackSize(ToSlot.Item, 1))
	{
		// Update Inventory
		ToSlot.Item.Quantity += 1;
		ContentsToModify.AddItem(SelectedItem.Name, 1);

		// Update Selection
		FInventoryItem NewSelection = SelectedItem;
		NewSelection.Quantity -= 1;
		Manipulator->StartDragging(NewSelection);
	}

	if (Manipulator->GetSelectedItem().Quantity <= 0)
	{
		Manipulator->StopDragging();
	}
}

bool UInventoryComponent::WithinStackSize(const FInventoryItem& BaseItem, const int32& AmountToAdd)
{
	return (BaseItem.Quantity + AmountToAdd) <= UWildOmissionStatics::GetItemData(BaseItem.Name)->StackSize;
}