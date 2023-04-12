// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveHandler.generated.h"

class UWildOmissionSaveGame;
class UWorldGenerationHandlerComponent;
class UActorSaveHandlerComponent;
class UPlayerSaveHandlerComponent;

UCLASS()
class WILDOMISSION_API ASaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveHandler();

	virtual void BeginPlay() override;

	void SaveGame();

	void SetSaveFile(const FString& SaveFileName);

	void LoadWorld();
	
	UPlayerSaveHandlerComponent* GetPlayerHandler();

	UWildOmissionSaveGame* GetSaveFile();

private:
	FString CurrentSaveFileName;

	UPROPERTY()
	UWorldGenerationHandlerComponent* WorldGenerationHandlerComponent;

	UPROPERTY()
	UActorSaveHandlerComponent* ActorSaveHandlerComponent;

	UPROPERTY()
	UPlayerSaveHandlerComponent* PlayerSaveHandlerComponent;
	
	void GenerateLevel(UWildOmissionSaveGame* SaveToModify);

	void ValidateSave();

	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);
};
