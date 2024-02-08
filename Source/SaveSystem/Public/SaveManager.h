// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveManager.generated.h"

UCLASS()
class SAVESYSTEM_API ASaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveManager();

	void SetGameSaveLoadController(class IGameSaveLoadController* InGameSaveLoadController);

	static ASaveManager* GetSaveManager();

	void SaveWorld();
	void SetSaveFile(const FString& SaveFileName);
	void LoadWorld();

	void UpdateSaveFile(class UWildOmissionSaveGame* UpdatedSaveFile);

	class UPlayerSaveManagerComponent* GetPlayerManager() const;
	class UWildOmissionSaveGame* GetSaveFile() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FString CurrentSaveFileName;

	UPROPERTY(VisibleAnywhere)
	class UPlayerSaveManagerComponent* PlayerSaveManagerComponent;

	class IGameSaveLoadController* GameSaveLoadController;

	void ValidateSave();

	UPROPERTY()
	class UWildOmissionSaveGame* CurrentSaveFile;

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