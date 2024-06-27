// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VitalsSaveData.h"
#include "InventorySaveData.h"
#include "Structs/AchievementStatsData.h"
#include "PlayerSaveData.generated.h"

USTRUCT()
struct SAVESYSTEM_API FPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	bool IsHost = false;

	UPROPERTY()
	bool IsAlive = false;

	UPROPERTY()
	bool NewPlayer = true;

	UPROPERTY()
	FString UniqueID = TEXT("");

	UPROPERTY()
	uint8 GameMode = 0;

	UPROPERTY()
	int32 BedUniqueID = -1;

	UPROPERTY()
	FIntVector2 BedChunkLocation = FIntVector2();

	UPROPERTY()
	FVector WorldLocation = FVector::ZeroVector;

	UPROPERTY()
	FVector LastDeathLocation = FVector::ZeroVector;

	UPROPERTY()
	FVector SavedLocation = FVector::ZeroVector;

	UPROPERTY()
	FVitalsSaveData Vitals;

	UPROPERTY()
	FInventorySaveData Inventory;

	UPROPERTY()
	TArray<uint8> SelectedItemByteData;

	UPROPERTY()
	FAchievementStatsData AchievementStatsData;

};
