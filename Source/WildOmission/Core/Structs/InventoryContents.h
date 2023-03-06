// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "InventoryItem.h"

#include "UObject/NoExportTypes.h"
#include "InventoryContents.generated.h"

USTRUCT()
struct FInventoryContents
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventoryItem> Contents;

	// Returns the amount of a given item in the inventory, will return 0 if item isn't present.
	int32 GetItemQuantity(const FName& ItemName)
	{
		int32 Index = GetItemIndex(ItemName);
		if (Index == INDEX_NONE)
		{
			return 0;
		}
		return Contents[Index].Quantity;
	}

	// Returns true if the specified item is present
	bool HasItem(const FName& ItemName)
	{
		return Contents.FindByPredicate([&ItemName](const FInventoryItem& ItemData) {
			return ItemData.CompareNames(ItemData, ItemName);
			}) != nullptr;
	}

	// Will add the given item and quantity to the list, if item is already present the quantity will be added to the existing.
	void AddItem(const FName& ItemName, const int32& QuantityToAdd)
	{
		if (HasItem(ItemName))
		{
			int32 ItemIndex = GetItemIndex(ItemName);
			Contents[ItemIndex].Quantity += QuantityToAdd;
		}
		else
		{
			FInventoryItem NewItem;
			NewItem.Name = ItemName;
			NewItem.Quantity = QuantityToAdd;
			Contents.Add(NewItem);
		}
	}

	void RemoveItem(const FName& ItemName, const int32& QuantityToRemove)
	{
		if (!HasItem(ItemName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot remove %s, item does not exist in inventory."), *ItemName.ToString());
			return;
		}

		int32 ItemIndex = GetItemIndex(ItemName);
		Contents[ItemIndex].Quantity -= QuantityToRemove;

		if (Contents[ItemIndex].Quantity <= 0)
		{
			Contents.RemoveAt(ItemIndex);
		}
	}

private:

	// Returns the index in the contents list of the given item
	int32 GetItemIndex(const FName& ItemName)
	{
		return Contents.IndexOfByPredicate([&ItemName](const FInventoryItem& ItemData) {
			return ItemData.CompareNames(ItemData, ItemName);
			});
	}

};
