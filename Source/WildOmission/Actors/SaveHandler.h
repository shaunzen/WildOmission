// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveHandler.generated.h"

class UWildOmissionSaveGame;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FString CurrentSaveFileName;

	void CreatePlayerSaves(TArray<APlayerController*> PlayerControllersToSave, TArray<struct FWildOmissionPlayerSave>& OutPlayerSaves);

	bool RetrivePlayerDataFromSave(FString PlayerUniqueID, FWildOmissionPlayerSave& OutPlayerSave);

	bool GetPlayerSaveIndex(FString PlayerUniqueID, int32& OutIndex);

	UWildOmissionSaveGame* GetSaveFile();
	void UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile);
};
