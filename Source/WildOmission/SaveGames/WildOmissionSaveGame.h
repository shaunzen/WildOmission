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

UCLASS()
class WILDOMISSION_API UWildOmissionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UWildOmissionSaveGame();

	UPROPERTY()
	TArray<FWildOmissionPlayerSave> PlayerSaves;
};
