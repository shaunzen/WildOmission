// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveHandler.generated.h"

class UWildOmissionSaveGame;
class UActorSaveHandlerComponent;
class UPlayerSaveHandlerComponent;
//class UWorldGenerationHandlerComponent;
class IGameSaveLoadController;

UCLASS()
class SAVESYSTEM_API ASaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveHandler();

	void Setup(IGameSaveLoadController* SaveLoadController);

	void SaveGame();

	void SetSaveFile(const FString& SaveFileName);

	void LoadWorld();
	
	UPlayerSaveHandlerComponent* GetPlayerHandler() const;
	//UWorldGenerationHandlerComponent* GetWorldGenerationHandler() const;

	UWildOmissionSaveGame* GetSaveFile();

private:
	FString CurrentSaveFileName;

	/*UPROPERTY(VisibleAnywhere)
	UWorldGenerationHandlerComponent* WorldGenerationHandlerComponent;*/

	UPROPERTY(VisibleAnywhere)
	UActorSaveHandlerComponent* ActorSaveHandlerComponent;

	UPROPERTY(VisibleAnywhere)
	UPlayerSaveHandlerComponent* PlayerSaveHandlerComponent;

	IGameSaveLoadController* GameSaveLoadController;

	//void GenerateLevel(UWildOmissionSaveGame* SaveToModify);

	//void RegenerateResources();

	void ValidateSave();

	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);

	UFUNCTION()
	void StopLoading();

};