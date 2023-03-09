// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WildOmissionPlayerController.generated.h"

class UInventoryComponent;

UCLASS()
class WILDOMISSION_API AWildOmissionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// TODO Save
	struct FWildOmissionPlayerSave SavePlayer();

	// TODO Load
	void LoadPlayerSave(const struct FWildOmissionPlayerSave& PlayerSave);

	// TODO SaveGame
	void Save();

	void Spawn();

	FString GetUniqueID();

	//*****************************
	// Testing functions

	UFUNCTION(Exec)
	void LogLocalInventoryContents();
	
private:
	UFUNCTION(Server, Reliable)
	void Server_AddToPendingSaves();
};
