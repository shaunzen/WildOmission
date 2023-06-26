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
	void Save(TArray<struct FWildOmissionPlayerSave>& OutUpdatedSavesList);

	// Adds the passed in PlayerControllers Save File to the pending list
	void AddToPending(APlayerController* PlayerController);

	// Finds the save data for the given player controller passed in, if save data is found it will automatically load.
	// If no save data is found it will result to spawning the player with default settings and items.
	void Load(APlayerController* PlayerController);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TArray<struct FWildOmissionPlayerSave> PendingSaves;

	UFUNCTION()
	void AddAllToPending();

	void AddSavesToList(const TArray<struct FWildOmissionPlayerSave>& InSaveList, TArray<struct FWildOmissionPlayerSave>& OutSavesList);
	
	void AddSaveToList(const struct FWildOmissionPlayerSave& InSave, TArray<struct FWildOmissionPlayerSave>& OutSavesList);
	
	bool GetSaveIndexInList(const TArray<struct FWildOmissionPlayerSave>& List, const FString& UniqueID, int32& OutIndex);
		
};
