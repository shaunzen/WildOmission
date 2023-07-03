// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSaveHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAVESYSTEM_API UPlayerSaveHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSaveHandlerComponent();

	// Will update the list passed in to include saves from the pending list.
	void Save(TArray<struct FPlayerSave>& OutUpdatedSavesList);

	// Adds the passed in PlayerControllers Save File to the pending list
	void AddToPending(APlayerController* PlayerController);

	// Finds the save data for the given player controller passed in, if save data is found it will automatically load.
	// If no save data is found it will result to spawning the player with default settings and items.
	void Load(APlayerController* PlayerController);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<struct FPlayerSave> PendingSaves;

	UFUNCTION()
	void AddAllToPending();

	void AddSavesToList(const TArray<struct FPlayerSave>& InSaveList, TArray<struct FPlayerSave>& OutSavesList);
	
	void AddSaveToList(const struct FPlayerSave& InSave, TArray<struct FPlayerSave>& OutSavesList);
	
	bool GetSaveIndexInList(const TArray<struct FPlayerSave>& List, const FString& UniqueID, int32& OutIndex);

	TArray<APlayerController*> GetAllPlayerControllers();

};
