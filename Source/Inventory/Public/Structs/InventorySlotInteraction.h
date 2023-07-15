// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventorySlotInteraction.generated.h"

class UInventoryManipulatorComponent;

USTRUCT()
struct FInventorySlotInteraction
{
	GENERATED_BODY()

	UPROPERTY()
	float Time = 0;

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	UInventoryManipulatorComponent* Manipulator = nullptr;

	UPROPERTY()
	bool QuickMove = false;

	UPROPERTY()
	bool Primary = true;

};