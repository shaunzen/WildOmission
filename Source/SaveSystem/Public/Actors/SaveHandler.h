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

UCLASS()
class SAVESYSTEM_API ASaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveHandler();

	void Setup(IGameSaveLoadController* SaveLoadController, IWorldGenerator* InWorldGenerator);

	void SaveGame();

	void SetSaveFile(const FString& SaveFileName);

	void LoadWorld();
	
	IWorldGenerator* GetWorldGenerator() const;
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
	IGameSaveLoadController* GameSaveLoadController;

	void ValidateSave();

	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);

	UFUNCTION()
	void StopLoading();

};