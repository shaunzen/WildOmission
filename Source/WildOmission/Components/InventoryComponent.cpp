// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryComponent.h"
#include "WildOmission/Widgets/PlayerUI/InventoryWidget.h"
#include "WildOmission/Actors/WorldItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// Make this component replicate
	SetIsReplicatedByDefault(true);

	// Find the item data table
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT("/Game/WildOmission/Blueprints/DataTables/ItemDataTable"));
	if (ItemDataTableObject.Succeeded())
	{
		ItemDataTable = ItemDataTableObject.Object;
	}
}

void UInventoryComponent::Setup(UInventoryWidget* InInventoryWidget)
{
	// Create a link between the Widget and this component
	InventoryWidget = InInventoryWidget;
	InventoryWidget->SetComponent(this);
}

void UInventoryComponent::AddItem(FName ItemName, int32 Quantity)
{
	int32 Remaining;
	int32 AmountAdded;
	bool AddSuccess = InventoryWidget->AddItem(ItemName, Quantity, Remaining);

	// Calculate how many were added
	AmountAdded = Quantity - Remaining;

	// Add item to item list
	Server_AddItem(ItemName, AmountAdded);

	if (AddSuccess == false)
	{
		SpawnWorldItem(ItemName, Remaining);
	}
}

void UInventoryComponent::Server_AddItem_Implementation(FName ItemName, int32 Quantity)
{
	// If the item already exists in the contents map
	if (int32* ItemQuantity = InventoryContents.Find(ItemName))
	{
		// Add the additional quantity to the existing quantity
		int32 NewQuantity = *ItemQuantity + Quantity;
		InventoryContents.Add(ItemName, NewQuantity);
	}
	else
	{
		// Create a new entry with that quantity
		InventoryContents.Add(ItemName, Quantity);
	}
}

void UInventoryComponent::RemoveItem(FName ItemName, int32 Quantity, bool bSpawnInWorld)
{
	Server_RemoveItem(ItemName, Quantity);
	if (bSpawnInWorld == true)
	{
		Server_SpawnWorldItem(ItemName, Quantity);
	}
	else
	{
		int32 Remaining;
		bool RemoveSuccess = InventoryWidget->RemoveItem(ItemName, Quantity, Remaining);
	}
}

bool UInventoryComponent::Server_RemoveItem_Validate(FName ItemName, int32 Quantity)
{
	// Only valid if the player has the item they are removing
	return InventoryContents.Find(ItemName) != nullptr;
}

void UInventoryComponent::Server_RemoveItem_Implementation(FName ItemName, int32 Quantity)
{
	// Find the item
	if (int32* ItemQuantity = InventoryContents.Find(ItemName))
	{
		// Remove the desired amount
		int32 NewQuantity = *ItemQuantity - Quantity;
		
		// If we dont have anymore after removing then remove from the our contents
		if (NewQuantity <= 0)
		{
			InventoryContents.Remove(ItemName);
		}
		else
		{
			// Set the new value
			InventoryContents.Add(ItemName, NewQuantity);
		}
	}
}

void UInventoryComponent::SwapItem()
{
	// TODO Swap item
}

void UInventoryComponent::SpawnWorldItem(FName ItemName, int32 Quantity)
{
	// RPC on the server to spawn a world item of out specification
	Server_SpawnWorldItem(ItemName, Quantity);
}

void UInventoryComponent::Server_SpawnWorldItem_Implementation(FName ItemName, int32 Quantity)
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

FItem* UInventoryComponent::GetItemData(FName ItemName)
{
	if (ItemDataTable == nullptr)
	{
		return nullptr;
	}
	static const FString ContextString(TEXT("Item Data Context"));
	return ItemDataTable->FindRow<FItem>(ItemName, ContextString, true);
}

TMap<FName, int32>* UInventoryComponent::GetContents()
{
	return &InventoryContents;
}

UInventoryWidget* UInventoryComponent::GetWidget()
{
	return InventoryWidget;
}