// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

USTRUCT()
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FName Name = FName("");

	UPROPERTY(VisibleAnywhere)
	int32 Quantity = 0;

	static bool CompareNames(const FInventoryItem& Item, const FName& ItemName);

	bool IsZero() const;
	
	void Set(const FName& InName, const int32& InQuantity);

	void Clear();
};