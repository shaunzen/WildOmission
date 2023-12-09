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
	void Save(TArray<struct FPlayerSaveData>& OutUpdatedSavesList);

	// Adds the passed in PlayerControllers Save File to the pending list
	void AddToPending(APlayerController* PlayerController);

	// Finds the save data for the given player controller passed in, if save data is found it will automatically load.
	// If no save data is found it will result to spawning the player with default settings and items.
	void Load(APlayerController* PlayerController);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<struct FPlayerSaveData> PendingSaves;

	UFUNCTION()
	void AddAllToPending();

	void AddSaveToList(const FPlayerSaveData& InSave, TArray<FPlayerSaveData>& OutSavesList);

	bool FindSaveIndexInList(const TArray<FPlayerSaveData>& SaveList, const FString& UniqueID, int32& OutIndex) const;
	bool FindHostSaveIndexInList(const TArray<FPlayerSaveData>& SaveList, int32& OutIndex) const;

	TArray<APlayerController*> GetAllPlayerControllers();

};
