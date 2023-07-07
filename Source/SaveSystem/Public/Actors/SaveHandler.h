// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveHandler.generated.h"

class UWildOmissionSaveGame;
class UActorSaveHandlerComponent;
class UPlayerSaveHandlerComponent;
class IGameSaveLoadController;
class IWorldGenerator;
class ISavableTimeOfDayHandler;
class ISavableWeatherHandler;

UCLASS()
class SAVESYSTEM_API ASaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveHandler();

	void Setup(IWorldGenerator* InWorldGenerator, ISavableTimeOfDayHandler* InTimeOfDayHandler, ISavableWeatherHandler* InWeatherHandler, IGameSaveLoadController* SaveLoadController);

	void SaveGame();

	void SetSaveFile(const FString& SaveFileName);

	void LoadWorld();
	
	IWorldGenerator* GetWorldGenerator() const;
	ISavableTimeOfDayHandler* GetTimeOfDayHandler() const;
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

	IWorldGenerator* WorldGenerator;
	ISavableTimeOfDayHandler* TimeOfDayHandler;
	ISavableWeatherHandler* WeatherHandler;
	IGameSaveLoadController* GameSaveLoadController;

	void ValidateSave();

	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);

	UFUNCTION()
	void StopLoading();

};