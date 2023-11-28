// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveHandler.generated.h"

class UWildOmissionSaveGame;
class UActorSaveHandlerComponent;
class UPlayerSaveHandlerComponent;
class IGameSaveLoadController;

UCLASS()
class SAVESYSTEM_API ASaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveHandler();

	void SetGameSaveLoadController(IGameSaveLoadController* InGameSaveLoadController);

	static ASaveHandler* GetSaveHandler();

	void SaveGame();
	void SetSaveFile(const FString& SaveFileName);
	void LoadWorld();

	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);

	UPlayerSaveHandlerComponent* GetPlayerHandler() const;
	UWildOmissionSaveGame* GetSaveFile() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FString CurrentSaveFileName;

	UPROPERTY(VisibleAnywhere)
	UActorSaveHandlerComponent* ActorSaveHandlerComponent;

	UPROPERTY(VisibleAnywhere)
	UPlayerSaveHandlerComponent* PlayerSaveHandlerComponent;

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