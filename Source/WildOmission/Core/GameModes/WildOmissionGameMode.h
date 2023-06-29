// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WildOmissionGameMode.generated.h"

class ASaveHandler;
class AWorldGenerationHandler;
class AWeatherHandler;
class AWildOmissionCharacter;
class AWildOmissionPlayerController;

UCLASS()
class WILDOMISSION_API AWildOmissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	void SpawnHumanForController(APlayerController* Controller);

	UFUNCTION()
	void OnControllerFinishedLoading(AWildOmissionPlayerController* LoadedController);

	UFUNCTION(BlueprintCallable, Exec)
	void SaveGame();

	UFUNCTION(BlueprintCallable, Exec)
	void ResetLocationOfAllConnectedPlayers();

	UFUNCTION(BlueprintCallable, Exec)
	void Weather(const FString& WeatherToSet);

	ASaveHandler* GetSaveHandler() const;
	AWorldGenerationHandler* GetWorldGenerationHandler() const;
	AWeatherHandler* GetWeatherHandler() const;

	UFUNCTION(Exec)
	void LogPlayerInventoryComponents();

	UFUNCTION(Exec)
	void LogPlayerInventorySlots();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> HumanCharacterClass;

	bool FriendsOnly = true;

	UPROPERTY()
	ASaveHandler* SaveHandler;

	UPROPERTY()
	AWorldGenerationHandler* WorldGenerationHandler;
	// TimeManager

	UPROPERTY()
	AWeatherHandler* WeatherHandler;
	
};
