// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSaveHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UPlayerSaveHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSaveHandlerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// this will take in the list of player saves from the save file and update and add the players from the pending list
	void SavePlayers(TArray<struct FWildOmissionPlayerSave>& OutUpdatedPlayerSaves);

	// this will find a players unique save data in the save file and tell it to load that data
	void LoadPlayer(APlayerController* PlayerController);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	// this will create save data for the given players passed in, then update, or add that data to the list passed in
	void CreatePlayerSaves(TArray<APlayerController*> PlayerControllersToSave, TArray<struct FWildOmissionPlayerSave>& OutPlayerSaves);

	// this will try to find the save data for a given Player ID in the save file, will return false if fails to find
	bool RetrivePlayerDataFromSave(FString PlayerUniqueID, struct FWildOmissionPlayerSave& OutPlayerSave);

	// this will return the index of a players save data in the save file, will return false if fails to find
	bool GetPlayerSaveIndex(FString PlayerUniqueID, int32& OutIndex);
		
};
