// Copyright Telephone Studios. All Rights Reserved.


#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "WorldItem.h"
#include "Net/UnrealNetwork.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "EngineUtils.h"

static UDataTable* ItemDataTable = nullptr;

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);

	DisplayName = TEXT("Inventory");

	SlotCount = 24;
	LoadedFromSave = false;

	// We have to add this stupid ugly check here, because unreal engine is a buggy broken mess somethimes.
	if (GetWorld())
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableBlueprint(TEXT("/Game/Inventory/DataTables/DT_Items"));
		if (ItemDataTableBlueprint.Succeeded())
		{
			ItemDataTable = ItemDataTableBlueprint.Object;
		}
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, ServerState, COND_OwnerOnly);
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

void UInventoryComponent::AddItem(const FInventoryItem& ItemToAdd, AActor* ActorToSpawnDropedItems, bool ForceClientUpdate)
{
	FInventoryItemUpdate AdditionItemUpdate;
	AdditionItemUpdate.Time = GetWorld()->GetRealTimeSeconds();
	AdditionItemUpdate.Addition = true;
	AdditionItemUpdate.Item = ItemToAdd;
	ServerState.Updates.Add(AdditionItemUpdate);
	
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

		FInventoryItemUpdate DroppedItemUpdate;
		DroppedItemUpdate.Time = GetWorld()->GetRealTimeSeconds();
		DroppedItemUpdate.Addition = false;
		DroppedItemUpdate.Item = DroppedItem;
		ServerState.Updates.Add(DroppedItemUpdate);

		AActor* DropperActor = nullptr;
		if (ActorToSpawnDropedItems == nullptr)
		{
			DropperActor = GetOwner();
		}
		else
		{
			DropperActor = ActorToSpawnDropedItems;
		}
		SpawnWorldItem(GetWorld(), DroppedItem, DropperActor);
	}
	
	if (ForceClientUpdate)
	{
		GetOwner()->ForceNetUpdate();
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

void UInventoryComponent::SlotInteraction(const int32& SlotIndex, UInventoryManipulatorComponent* Manipulator, bool QuickMove, bool Primary)
{
	// Create an interaction
	FInventorySlotInteraction CurrentInteraction;
	CurrentInteraction.SlotIndex = SlotIndex;
	CurrentInteraction.Manipulator = Manipulator;
	CurrentInteraction.QuickMove = QuickMove;
	CurrentInteraction.Primary = Primary;
	CurrentInteraction.Time = GetWorld()->GetRealTimeSeconds();

	// Send it to the server
	Server_SlotInteraction(CurrentInteraction);

	// Add it to our unacknowlaged interactions
	UnacknowalgedInteractions.Add(CurrentInteraction);

	if (GetOwner()->HasAuthority())
	{
		return;
	}

	HandleSlotInteraction(CurrentInteraction, false);

	BroadcastInventoryUpdate();
}

FItemData* UInventoryComponent::GetItemData(const FName& ItemID)
{
	if (ItemDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Item Data Context"));

	return ItemDataTable->FindRow<FItemData>(ItemID, ContextString, true);
}

void UInventoryComponent::SpawnWorldItem(UWorld* WorldContextObject, const FInventoryItem& ItemToSpawn, AActor* SpawningActor)
{
	if (WorldContextObject == nullptr || SpawningActor == nullptr)
	{
		return;
	}

	// Get the data for this item
	FItemData* ItemData = GetItemData(ItemToSpawn.Name);

	// Spawn a world item actor
	AWorldItem* WorldItem = WorldContextObject->SpawnActor<AWorldItem>();
	if (ItemData == nullptr || WorldItem == nullptr)
	{
		return;
	}

	WorldItem->SetOwner(SpawningActor);

	FVector SpawnLocation;
	FVector PhysicsImpulse;

	SpawnLocation = SpawningActor->GetActorLocation();
	PhysicsImpulse = SpawningActor->GetActorForwardVector() * 5000.0f;

	// Update world items properties
	WorldItem->SetActorLocation(SpawnLocation);
	WorldItem->SetItem(ItemToSpawn);
	WorldItem->AddImpulse(PhysicsImpulse);
}

void UInventoryComponent::OnLoadComplete_Implementation()
{
	ServerState.Contents = Contents;
	ServerState.Slots = Slots;
	LoadedFromSave = true;
}

void UInventoryComponent::HandleSlotInteraction(const FInventorySlotInteraction& Interaction, bool UseServerState)
{
	if (Interaction.QuickMove == true)
	{
		// are we the manipulators inventory?
		if (Interaction.Manipulator->GetOwnersInventory() == this) // Interaction from within the players inventory
		{
			if (Interaction.Manipulator->GetOpenContainer() == nullptr) // We are quick moving within the players inventory
			{
				// Store the item information in this slot
				FInventoryItem MovingItem = UseServerState ? ServerState.Slots[Interaction.SlotIndex].Item : Slots[Interaction.SlotIndex].Item;
				
				// Remove the item from said slot
				UseServerState ? ServerState.Slots[Interaction.SlotIndex].ClearItem() : Slots[Interaction.SlotIndex].ClearItem();

				// Use one of the find slot functions to find a better slot and move the item to the new slot
				int32 Remaining = 0; // THIS ISNT USED HERE
				AddItemToSlots(MovingItem, Remaining);
			}
			else // We are quick moving from players inventory to an item container
			{
				// Store ItemData
				FInventoryItem MovingItem = UseServerState ? ServerState.Slots[Interaction.SlotIndex].Item : Slots[Interaction.SlotIndex].Item;

				// Remove the item from our inventory
				UseServerState ? ServerState.Contents.RemoveItem(MovingItem.Name, MovingItem.Quantity) : Contents.RemoveItem(MovingItem.Name, MovingItem.Quantity);
				UseServerState ? ServerState.Slots[Interaction.SlotIndex].ClearItem() : Slots[Interaction.SlotIndex].ClearItem();

				// Add item to container inventory
				Interaction.Manipulator->GetOpenContainer()->AddItem(MovingItem);
			}
		}
		else if (Interaction.Manipulator->GetOpenContainer() && Interaction.Manipulator->GetOpenContainer() == this) // Interaction from within an open container to players inventory
		{
			// Store the item information
			FInventoryItem MovingItem = UseServerState ? ServerState.Slots[Interaction.SlotIndex].Item : Slots[Interaction.SlotIndex].Item;

			// Remove the item from the open container
			UseServerState ? ServerState.Contents.RemoveItem(MovingItem.Name, MovingItem.Quantity) : Contents.RemoveItem(MovingItem.Name, MovingItem.Quantity);
			UseServerState ? ServerState.Slots[Interaction.SlotIndex].ClearItem() : Slots[Interaction.SlotIndex].ClearItem();

			// Add the item to player inventory
			Interaction.Manipulator->GetOwnersInventory()->AddItem(MovingItem);
		}
		// or are we the open container?
		UE_LOG(LogTemp, Warning, TEXT("Processing QuickMove action."));
		return;
	}

	if (!Interaction.Manipulator->IsDragging())
	{
		if (Interaction.Primary)
		{
			DragAll(Interaction.SlotIndex, Interaction.Manipulator, UseServerState ? ServerState.Slots : Slots, UseServerState ? ServerState.Contents : Contents);
		}
		else
		{
			DragSplit(Interaction.SlotIndex, Interaction.Manipulator, UseServerState ? ServerState.Slots : Slots, UseServerState ? ServerState.Contents : Contents);
		}
	}
	else
	{
		if (Interaction.Primary)
		{
			DropAll(Interaction.SlotIndex, Interaction.Manipulator, UseServerState ? ServerState.Slots : Slots, UseServerState ? ServerState.Contents : Contents);
		}
		else
		{
			DropSingle(Interaction.SlotIndex, Interaction.Manipulator, UseServerState ? ServerState.Slots : Slots, UseServerState ? ServerState.Contents : Contents);
		}
	}
}

void UInventoryComponent::Server_SlotInteraction_Implementation(const FInventorySlotInteraction& Interaction)
{
	// Update our server state
	ServerState.LastInteraction = Interaction;

	HandleSlotInteraction(Interaction, true);

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

FString UInventoryComponent::GetDisplayName() const
{
	return DisplayName;
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

void UInventoryComponent::SetDisplayName(const FString& NewDisplayName)
{
	DisplayName = NewDisplayName;
}

void UInventoryComponent::SetSlotCount(const uint8& InSlotCount)
{
	SlotCount = InSlotCount;
}

bool UInventoryComponent::WasLoadedFromSave() const
{
	return LoadedFromSave;
}

TArray<uint8> UInventoryComponent::Save()
{
	TArray<uint8> Bytes;

	FMemoryWriter MemoryWriter(Bytes);
	FObjectAndNameAsStringProxyArchive InventoryArchive(MemoryWriter, true);
	InventoryArchive.ArIsSaveGame = true;
	this->Serialize(InventoryArchive);

	return Bytes;
}

void UInventoryComponent::Load(const TArray<uint8>& InSave)
{
	FMemoryReader MemoryReader(InSave);
	FObjectAndNameAsStringProxyArchive InventoryArchive(MemoryReader, true);
	InventoryArchive.ArIsSaveGame = true;
	this->Serialize(InventoryArchive);

	OnLoadComplete_Implementation();
	OnRep_ServerState();
}

void UInventoryComponent::OnRep_ServerState()
{
	ClearAcknowlagedInteractions(ServerState.LastInteraction);
	
	TArray<FInventoryItemUpdate> UnacknowlagedUpdates;
	GetUnacknowlagedUpdates(UnacknowlagedUpdates);
	BroadcastAllUpdateNotifications(UnacknowlagedUpdates);

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

void UInventoryComponent::GetUnacknowlagedUpdates(TArray<FInventoryItemUpdate>& OutUpdatesList)
{
	for (const FInventoryItemUpdate& Update : ServerState.Updates)
	{
		if (Update.Time <= LastClientAcknowlagedItemUpdate.Time)
		{
			continue;
		}
		OutUpdatesList.Add(Update);
	}

	if (ServerState.Updates.IsEmpty())
	{
		return;
	}
	
	LastClientAcknowlagedItemUpdate = ServerState.Updates[ServerState.Updates.Num() - 1];
}

void UInventoryComponent::BroadcastAllUpdateNotifications(const TArray<FInventoryItemUpdate>& UpdatesList)
{
	for (const FInventoryItemUpdate& ItemUpdate : UpdatesList)
	{
		BroadcastItemUpdate(ItemUpdate);
	}
}

void UInventoryComponent::BroadcastInventoryUpdate()
{
	if (!OnUpdate.IsBound())
	{
		return;
	}

	OnUpdate.Broadcast();
}

void UInventoryComponent::BroadcastItemUpdate(const FInventoryItemUpdate& ItemUpdate)
{
	if (!OnItemUpdate.IsBound())
	{
		return;
	}

	OnItemUpdate.Broadcast(ItemUpdate);
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
	for (FInventorySlot& Slot : ServerState.Slots)
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
	else if (ToSlot.SameItemNameAs(SelectedItem) && GetItemData(SelectedItem.Name)->StackSize != 1) // same item and the stack size for this item is not 1
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
			int32 AmountAdded = GetItemData(ToSlot.Item.Name)->StackSize - ToSlot.Item.Quantity;
			ToSlot.Item.Quantity = GetItemData(SelectedItem.Name)->StackSize;
			ContentsToModify.AddItem(SelectedItem.Name, AmountAdded);
			
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
	return (BaseItem.Quantity + AmountToAdd) <= GetItemData(BaseItem.Name)->StackSize;
}