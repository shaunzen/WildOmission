// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WildOmissionSaveGame.generated.h"

USTRUCT()
struct FWildOmissionPlayerSave
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bHost = false;

	UPROPERTY()
	int32 ID = 0;
	
	UPROPERTY()
	FVector WorldLocation = FVector::ZeroVector;

	UPROPERTY()
	TMap<FName, int32> InventoryContents;
	
	UPROPERTY()
	TArray<class UInventorySlotWidget*> InventorySlots;

};

USTRUCT()
struct FWildOmissionSaveCreationInformation
{
	GENERATED_BODY()

	UPROPERTY()
	bool bLevelHasGenerated = false;

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
