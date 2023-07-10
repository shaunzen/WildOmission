// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventorySlotInteraction.h"
#include "InventoryContents.h"
#include "InventorySlot.h"
#include "InventoryItemUpdate.h"
#include "InventoryState.generated.h"

USTRUCT()
struct FInventoryState
{
	GENERATED_BODY()

	UPROPERTY()
	FInventorySlotInteraction LastInteraction;

	UPROPERTY()
	FInventoryContents Contents;

	UPROPERTY()
	TArray<FInventorySlot> Slots;
	
	UPROPERTY()
	TArray<FInventoryItemUpdate> Updates;

};