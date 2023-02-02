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

	UPROPERTY()
	FInventoryItem Item;
	
	UPROPERTY()
	int32 Index;

	void SetItem(const FInventoryItem& InItem);

	void ClearItem();

	bool CompareItem(const FInventoryItem& ItemToCompare) const;

	bool IsEmpty() const;
};