// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveHandler.generated.h"

class UWildOmissionSaveGame;
class UActorSaveHandlerComponent;
class UPlayerSaveHandlerComponent;
class IGameSaveLoadController;
class ISavableWeatherHandler;

UCLASS()
class SAVESYSTEM_API ASaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveHandler();

	void Setup(ISavableWeatherHandler* InWeatherHandler, IGameSaveLoadController* SaveLoadController);

	void SaveGame();

	void SetSaveFile(const FString& SaveFileName);

	void LoadWorld();
	
	ISavableWeatherHandler* GetWeatherHandler() const;
	IGameSaveLoadController* GetSaveLoadController() const;

	UPlayerSaveHandlerComponent* GetPlayerHandler() const;
	UWildOmissionSaveGame* GetSaveFile();

private:
	FString CurrentSaveFileName;

	UPROPERTY(VisibleAnywhere)
	UActorSaveHandlerComponent* ActorSaveHandlerComponent;

	UPROPERTY(VisibleAnywhere)
	UPlayerSaveHandlerComponent* PlayerSaveHandlerComponent;

	ISavableWeatherHandler* WeatherHandler;
	IGameSaveLoadController* GameSaveLoadController;

	void ValidateSave();

	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);

	UFUNCTION()
	void StopLoading();

};