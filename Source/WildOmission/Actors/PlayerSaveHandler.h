// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSaveHandler.generated.h"

class ASaveHandler;

UCLASS()
class WILDOMISSION_API APlayerSaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSaveHandler();

	void Setup(ASaveHandler* InOwner);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// this will take in the list of player saves from the save file and update and add the players from the pending list
	void SavePlayers(TArray<struct FWildOmissionPlayerSave>& OutUpdatedPlayerSaves);

	// this will find a players unique save data in the save file and tell it to load that data
	void LoadPlayer(APlayerController* PlayerController);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ASaveHandler* OwnerSaveHandler;

	// this will create save data for the given players passed in, then update, or add that data to the list passed in
	void CreatePlayerSaves(TArray<APlayerController*> PlayerControllersToSave, TArray<struct FWildOmissionPlayerSave>& OutPlayerSaves);
	
	// this will try to find the save data for a given Player ID in the save file, will return false if fails to find
	bool RetrivePlayerDataFromSave(FString PlayerUniqueID, struct FWildOmissionPlayerSave& OutPlayerSave);

	// this will return the index of a players save data in the save file, will return false if fails to find
	bool GetPlayerSaveIndex(FString PlayerUniqueID, int32& OutIndex);
};
