// (c) 2023 Telephone Studios, all rights reserved.


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

void UInventoryComponent::AddItem(FName ItemName, int32 Quantity)
{
	// Add item to item list
	if (int32* ItemQuantity = InventoryContent.Find(ItemName))
	{
		int32 NewQuantity = *ItemQuantity + Quantity;
		InventoryContent.Add(ItemName, NewQuantity);
	}
	else
	{
		InventoryContent.Add(ItemName, Quantity);
	}

	// Add item to widget
	InventoryWidget->AddItem(ItemName, Quantity);
}

void UInventoryComponent::RemoveItem()
{
	// TODO Remove item
}

void UInventoryComponent::SwapItem()
{
	// TODO Swap item
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