// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VitalsSave.h"
#include "InventorySave.h"
#include "PlayerSave.generated.h"

USTRUCT()
struct SAVESYSTEM_API FSelectedItemDataSave
{
	GENERATED_BODY();
};

USTRUCT()
struct SAVESYSTEM_API FPlayerSave
{
	GENERATED_BODY()

	UPROPERTY()
	bool IsHost = false;

	UPROPERTY()
	bool IsAlive = false;

	UPROPERTY()
	FString UniqueID = FString("");

	UPROPERTY()
	FVector WorldLocation = FVector::ZeroVector;

	UPROPERTY()
	FVitalsSave Vitals;

	UPROPERTY()
	FInventorySave Inventory;

	UPROPERTY()
	FInventoryItem SelectedItem;

};
