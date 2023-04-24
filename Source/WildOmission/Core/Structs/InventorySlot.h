// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.h"
#include "InventorySlot.generated.h"

USTRUCT()
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FInventoryItem Item = FInventoryItem();
	
	UPROPERTY(SaveGame)
	int32 Index = 0;

	void SetItem(const FInventoryItem& InItem);

	void ClearItem();

	bool SameItemNameAs(const FInventoryItem& ItemToCompare) const;

	bool SameItemIdAs(const FInventoryItem& ItemToCompare) const;

	bool IsEmpty() const;
};