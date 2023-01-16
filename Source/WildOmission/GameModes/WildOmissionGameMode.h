// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WildOmissionGameMode.generated.h"

class UWildOmissionSaveGame;

UCLASS()
class WILDOMISSION_API AWildOmissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void LogPlayerInventoryComponents();

	UFUNCTION(BlueprintCallable, Exec)
		void SaveGame();
	UFUNCTION(BlueprintCallable, Exec)
		void LoadGame();
private:
	UWildOmissionSaveGame* WildOmissionSaveGame;
	FVector GetPlayerLocation();
	void SetPlayerLocation(FVector InLocation);
};
