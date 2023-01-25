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

	// Will update the list passed in to include saves from the pending list.
	void SavePlayers(TArray<struct FWildOmissionPlayerSave>& OutUpdatedPlayerSaves);

	// Finds the save data for the given player controller passed in, if save data is found it will automatically load.
	// If no save data is found it will result to spawning the player with default settings and items.
	void LoadPlayer(APlayerController* PlayerController);
protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	TArray<struct FWildOmissionPlayerSave> PendingSaves;

	UFUNCTION()
	void UpdatePendingList();

	void AddPendingSavesToList(TArray<struct FWildOmissionPlayerSave>& OutListToAddTo);

	// this will create save data for the given players passed in, then update, or add that data to the list passed in
	void CreatePlayerSaves(TArray<APlayerController*> PlayerControllersToSave, TArray<struct FWildOmissionPlayerSave>& OutPlayerSaves);

	// this will try to find the save data for a given Player ID in the save file, will return false if fails to find
	bool RetrivePlayerDataFromSave(const FString& PlayerUniqueID, struct FWildOmissionPlayerSave& OutPlayerSave);

	// this will return the index of a players save data in the specified list, will return false if fails to find
	bool GetPlayerIndexInList(const TArray<struct FWildOmissionPlayerSave>& List, const FString& PlayerUniqueID, int32& OutIndex);
		
};
