// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.h"
#include "InventoryItemUpdate.generated.h"

USTRUCT()
struct FInventoryItemUpdate
{
	GENERATED_BODY()

	UPROPERTY()
	float Time = 0;

	UPROPERTY()
	bool Addition = true;

	UPROPERTY()
	FInventoryItem Item;
};