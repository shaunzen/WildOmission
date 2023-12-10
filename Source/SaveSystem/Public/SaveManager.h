// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveManager.generated.h"

class UWildOmissionSaveGame;
class UActorSaveManagerComponent;
class UPlayerSaveManagerComponent;
class IGameSaveLoadController;

UCLASS()
class SAVESYSTEM_API ASaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveManager();

	void SetGameSaveLoadController(IGameSaveLoadController* InGameSaveLoadController);

	static ASaveManager* GetSaveManager();

	void SaveGame();
	void SetSaveFile(const FString& SaveFileName);
	void LoadWorld();

	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);

	UPlayerSaveManagerComponent* GetPlayerManager() const;
	UWildOmissionSaveGame* GetSaveFile() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FString CurrentSaveFileName;

	UPROPERTY(VisibleAnywhere)
	UActorSaveManagerComponent* ActorSaveManagerComponent;

	UPROPERTY(VisibleAnywhere)
	UPlayerSaveManagerComponent* PlayerSaveManagerComponent;

	IGameSaveLoadController* GameSaveLoadController;

	void ValidateSave();

	UFUNCTION()
	void MarkSaveGenerated();

	UPROPERTY()
	UWildOmissionSaveGame* CurrentSaveFile;

	UFUNCTION()
	void StartLoading();

	UFUNCTION()
	void StopLoading();

	UFUNCTION()
	void SetLoadingTitle(const FString& NewTitle);

	UFUNCTION()
	void SetLoadingSubtitle(const FString& NewSubtitle);

	UFUNCTION()
	void CreateWorld(const FString& NewWorldName);

};