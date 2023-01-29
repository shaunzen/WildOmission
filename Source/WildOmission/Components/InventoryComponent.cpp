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
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Contents);
}

void UInventoryComponent::Setup(UInventoryWidget* InInventoryWidget)
{
	// Create a link between the Widget and this component
	InventoryWidget = InInventoryWidget;
	InventoryWidget->SetComponent(this);
}

void UInventoryComponent::AddItem(const FName& ItemName, const int32& Quantity)
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
		bool RemoveSuccess = InventoryWidget->RemoveItem(ItemName, Quantity, Remaining);
	}
}

bool UInventoryComponent::Server_RemoveItem_Validate(const FName& ItemName, const int32& Quantity)
{
	// Only valid if the player has the item they are removing
	return !Contents.HasItem(ItemName);
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