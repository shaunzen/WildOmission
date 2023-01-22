// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveHandler.generated.h"

class UWildOmissionSaveGame;
class APlayerSaveHandler;

UCLASS()
class WILDOMISSION_API ASaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveHandler();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SaveGame();

	void LoadGame(const FString& SaveFileName);
	
	void LoadPlayer(APlayerController* PlayerController);

	UWildOmissionSaveGame* GetSaveFile();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FString CurrentSaveFileName;

	APlayerSaveHandler* PlayerSaveHandler;

	void SavePendingPlayers(TArray<struct FWildOmissionPlayerSave>& OutPlayerSaves);

	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);
};
