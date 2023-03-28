// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveHandler.generated.h"

class UWildOmissionSaveGame;
class UPlayerSaveHandlerComponent;
class UWorldItemSaveHandlerComponent;

UCLASS()
class WILDOMISSION_API ASaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveHandler();

	virtual void BeginPlay() override;

	void SaveGame();

	void LoadGame(const FString& SaveFileName);

	UPlayerSaveHandlerComponent* GetPlayerHandler();

	UWildOmissionSaveGame* GetSaveFile();

private:
	FString CurrentSaveFileName;

	UPROPERTY()
	UPlayerSaveHandlerComponent* PlayerSaveHandlerComponent;
	
	UPROPERTY()
	UWorldItemSaveHandlerComponent* WorldItemSaveHandlerComponent;

	void ValidateSave();

	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);
};
