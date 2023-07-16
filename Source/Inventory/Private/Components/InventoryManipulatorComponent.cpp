// Copyright Telephone Studios. All Rights Reserved.


#include "Components/InventoryManipulatorComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "WorldItem.h"
#include "Net/UnrealNetwork.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UInventoryManipulatorComponent::UInventoryManipulatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Dragging = false;
}

void UInventoryManipulatorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryManipulatorComponent, OpenContainer, COND_OwnerOnly);
}

// Called when the game starts
void UInventoryManipulatorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner() == nullptr)
	{
		return;
	}

	OwnersInventory = GetOwner()->FindComponentByClass<UInventoryComponent>();

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	Dragging = false;
	SelectedItem.Clear();
}

//**************************************************************
// General Management
//**************************************************************

void UInventoryManipulatorComponent::StartDragging(const FInventoryItem& ItemToDrag)
{
	SelectedItem = ItemToDrag;
	Dragging = true;

	BroadcastSelectionChanged();
}

void UInventoryManipulatorComponent::StopDragging()
{
	if (Dragging == false)
	{
		return;
	}

	SelectedItem.Clear();
	Dragging = false;

	BroadcastSelectionChanged();
}

//**************************************************************
// User Interaction
//**************************************************************

void UInventoryManipulatorComponent::DropSelectedItemInWorld(bool Single)
{
	Server_DropSelectedItemInWorld(Single);

	if (!Dragging || GetOwner()->HasAuthority())
	{
		return;
	}

	FInventoryItem ItemToSpawn;
	ItemToSpawn = SelectedItem;

	if (Single == true)
	{
		ItemToSpawn.Quantity = 1;
		SelectedItem.Quantity -= 1;
	}
	else
	{
		SelectedItem.Clear();
	}

	if (SelectedItem.Quantity == 0)
	{
		StopDragging();
	}

	BroadcastSelectionChanged();
}

void UInventoryManipulatorComponent::Server_DropSelectedItemInWorld_Implementation(bool Single)
{
	if (!Dragging)
	{
		return;
	}

	FInventoryItem ItemToSpawn;
	ItemToSpawn = SelectedItem;

	if (Single == true)
	{
		ItemToSpawn.Quantity = 1;
		UInventoryComponent::SpawnWorldItem(GetWorld(), ItemToSpawn, GetOwner());
		SelectedItem.Quantity -= 1;
	}
	else
	{
		UInventoryComponent::SpawnWorldItem(GetWorld(), ItemToSpawn, GetOwner());
		SelectedItem.Clear();
	}

	if (SelectedItem.Quantity == 0)
	{
		StopDragging();
	}

	BroadcastSelectionChanged();
}

//**************************************************************
// Getters
//**************************************************************

bool UInventoryManipulatorComponent::IsDragging() const
{
	return Dragging;
}

UInventoryComponent* UInventoryManipulatorComponent::GetOpenContainer() const
{
	return OpenContainer;
}

UInventoryComponent* UInventoryManipulatorComponent::GetOwnersInventory() const
{
	return OwnersInventory;
}

void UInventoryManipulatorComponent::Server_SetOpenContainer_Implementation(UInventoryComponent* InOpenContainer)
{
	OpenContainer = InOpenContainer;
	OnRep_OpenContainer();
}

bool UInventoryManipulatorComponent::SelectedItemHasUniqueID(const uint32& UniqueID) const
{
	if (Dragging == false || SelectedItem.UniqueID != UniqueID)
	{
		return false;
	}

	return true;
}

TArray<uint8> UInventoryManipulatorComponent::GetSelectedItemAsByteData()
{
	TArray<uint8> Bytes;

	FMemoryWriter MemoryWriter(Bytes);
	FObjectAndNameAsStringProxyArchive SelectedItemArchive(MemoryWriter, true);
	SelectedItemArchive.ArIsSaveGame = true;
	this->Serialize(SelectedItemArchive);

	return Bytes;
}

void UInventoryManipulatorComponent::LoadSelectedItemFromByteDataAndDropInWorld(const TArray<uint8>& ByteData)
{
	FMemoryReader MemoryReader(ByteData);
	FObjectAndNameAsStringProxyArchive SelectedItemArchive(MemoryReader, true);
	SelectedItemArchive.ArIsSaveGame = true;
	this->Serialize(SelectedItemArchive);

	if (SelectedItem.Quantity > 0)
	{
		UInventoryComponent::SpawnWorldItem(GetWorld(), SelectedItem, GetOwner());
		SelectedItem.Clear();
	}
}

FInventoryItem UInventoryManipulatorComponent::GetSelectedItem()
{
	return SelectedItem;
}

FInventoryItem* UInventoryManipulatorComponent::GetSelectedItemAddress()
{
	return &SelectedItem;
}

void UInventoryManipulatorComponent::OnRep_OpenContainer()
{
	if (!OnOpenContainerChanged.IsBound())
	{
		return;
	}

	OnOpenContainerChanged.Broadcast(OpenContainer);
}

void UInventoryManipulatorComponent::BroadcastSelectionChanged()
{
	if (!OnSelectionChanged.IsBound())
	{
		return;
	}

	OnSelectionChanged.Broadcast(SelectedItem);
}