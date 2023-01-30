// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WildOmission/Structs/InventoryItem.h"
#include "WildOmissionSaveGame.generated.h"

USTRUCT()
struct FInventorySlotSave
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Index;

	UPROPERTY()
	FInventoryItem Contents;

};

USTRUCT()
struct FWildOmissionInventorySave
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<FInventoryItem> Items;
	
	UPROPERTY()
	TArray<FInventorySlotSave> Slots;
};

USTRUCT()
struct FWildOmissionPlayerSave
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
	FWildOmissionInventorySave Inventory;

};

USTRUCT()
struct FWildOmissionSaveCreationInformation
{
	GENERATED_BODY()

	UPROPERTY()
	bool LevelHasGenerated = false;

	UPROPERTY()
	uint8 Day = 1;
	
	UPROPERTY()
	uint8 Month = 1;
	
	UPROPERTY()
	uint16 Year = 2023;

};

UCLASS()
class WILDOMISSION_API UWildOmissionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UWildOmissionSaveGame();

	UPROPERTY()
	uint32 DaysPlayed;
	
	UPROPERTY()
	FWildOmissionSaveCreationInformation CreationInformation;

	UPROPERTY()
	TArray<FWildOmissionPlayerSave> PlayerSaves;

};
