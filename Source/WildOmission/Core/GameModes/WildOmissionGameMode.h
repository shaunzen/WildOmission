// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WildOmissionGameMode.generated.h"

class ASaveHandler;
class AWildOmissionCharacter;

UCLASS()
class WILDOMISSION_API AWildOmissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable, Exec)
	void SaveGame();

	UFUNCTION(BlueprintCallable, Exec)
	void ResetLocationOfAllConnectedPlayers();

	ASaveHandler* GetSaveHandler();

	TArray<APlayerController*> GetAllPlayerControllers();

	UFUNCTION(Exec)
	void LogPlayerInventoryComponents();

	UFUNCTION(Exec)
	void LogPlayerInventorySlots();

private:
	UPROPERTY()
	ASaveHandler* SaveHandler;
	
};
