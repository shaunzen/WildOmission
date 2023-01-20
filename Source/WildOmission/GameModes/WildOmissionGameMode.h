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
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(Exec)
	void LogPlayerInventoryComponents();

	UFUNCTION(BlueprintCallable, Exec)
	void SaveGame();
	
	UFUNCTION(BlueprintCallable, Exec)
	void LoadGame();

	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	UWildOmissionSaveGame* WildOmissionSaveGame;

	FString CurrentSaveName;

	void SavePlayers(TArray<struct FWildOmissionPlayerSave>& OutPlayerSaves);
	void LoadAllPlayers(const TArray<struct FWildOmissionPlayerSave>& PlayerSaves);
	void LoadPlayer(int32 PlayerID, APlayerController* PlayerController);
};
