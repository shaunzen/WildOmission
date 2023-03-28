// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "WildOmission/Core/Structs/InventorySlot.h"
#include "WildOmissionSaveGame.generated.h"

USTRUCT()
struct FWildOmissionInventorySave
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FInventoryItem> Items;
	
	UPROPERTY()
	TArray<FInventorySlot> Slots;

};

USTRUCT()
struct FVitalsSave
{
	GENERATED_BODY()

	UPROPERTY()
	float Health;
	
	UPROPERTY()
	float Hunger;
	
	UPROPERTY()
	float Thirst;
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

	UPROPERTY()
	FVitalsSave Vitals;

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
