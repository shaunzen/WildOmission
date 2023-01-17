// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WildOmission/SaveGames/WildOmissionSaveGame.h"
#include "WildOmissionPlayerController.generated.h"

class UInventoryComponent;

UCLASS()
class WILDOMISSION_API AWildOmissionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FWildOmissionPlayerSave SavePlayer();

	void LoadPlayerSave(const FWildOmissionPlayerSave& PlayerSave);

	UFUNCTION(Exec)
	void LogLocalInventoryContents();

	UFUNCTION(Server, Reliable)
	void Server_DestroyActor(AActor* ActorToDestroy);

};
