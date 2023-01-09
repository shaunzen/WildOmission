// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryComponent.h"
#include "../Widgets/InventoryWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT("/Script/Engine.DataTable'/Game/Blueprints/ItemDataTable.ItemDataTable'"));
	if (ItemDataTableObject.Succeeded())
	{
		ItemDataTable = ItemDataTableObject.Object;
	}
}

void UInventoryComponent::Setup(UInventoryWidget* InInventoryWidget)
{
	InventoryWidget = InInventoryWidget;
	InventoryWidget->SetComponent(this);
}

// Client side check for free slot, once this calculation has been completed call the server to add to the data

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
		// Spawn a world item with remaining count
		// Server should spawn world items
		SpawnWorldItem(ItemName, Remaining);
		UE_LOG(LogTemp, Warning, TEXT("Couldn't add all items. %i were remaining to be added."), Remaining);
	}
}

void UInventoryComponent::Server_AddItem_Implementation(FName ItemName, int32 Quantity)
{
	if (int32* ItemQuantity = InventoryContent.Find(ItemName))
	{
		int32 NewQuantity = *ItemQuantity + Quantity;
		InventoryContent.Add(ItemName, NewQuantity);
	}
	else
	{
		InventoryContent.Add(ItemName, Quantity);
	}
}

void UInventoryComponent::RemoveItem()
{
	// TODO Remove item
}

void UInventoryComponent::SwapItem()
{
	// TODO Swap item
}

void UInventoryComponent::SpawnWorldItem(FName ItemName, int32 Quantity)
{
	// setup world item actor
	// spawn it
	UE_LOG(LogTemp, Warning, TEXT("Spawning world item with id: %s and quantity of: %i"), *ItemName.ToString(), Quantity);
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

int32 UInventoryComponent::GetMaxSize()
{
	return MaxSize;
}

TMap<FName, int32>* UInventoryComponent::GetContent()
{
	return &InventoryContent;
}

void UInventoryComponent::SetMaxSize(int32 InMaxSize)
{
	MaxSize = InMaxSize;
}

UInventoryWidget* UInventoryComponent::GetWidget()
{
	return InventoryWidget;
}