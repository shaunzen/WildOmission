// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "UObject/NoExportTypes.h"
#include "InventoryContents.generated.h"

USTRUCT()
struct INVENTORY_API FInventoryContents
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, SaveGame)
	TArray<FInventoryItem> Contents;

	// Returns the amount of a given item in the inventory, will return 0 if item isn't present.
	int32 GetItemQuantity(const FName& ItemName);

	// Returns true if the specified item is present
	bool HasItem(const FName& ItemName);

	// Will add the given item and quantity to the list, if item is already present the quantity will be added to the existing.
	void AddItem(const FName& ItemName, const int32& QuantityToAdd);

	// Will remove the specified amount of the item passed in from the list
	void RemoveItem(const FName& ItemName, const int32& QuantityToRemove);

private:
	// Returns the index in the contents list of the given item
	int32 GetItemIndex(const FName& ItemName);

};
