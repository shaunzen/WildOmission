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
	struct FWildOmissionPlayerSave SavePlayer();

	void LoadPlayerSave(const struct FWildOmissionPlayerSave& PlayerSave);

	void Save();

	void Spawn();

	FString GetUniqueID();

	UFUNCTION(Exec)
	void LogLocalInventoryContents();
	UFUNCTION(Exec)
	void LogLocalInventorySlots();

	UFUNCTION(Server, Reliable)
	void Server_DestroyActor(AActor* ActorToDestroy);

private:
	UFUNCTION(Server, Reliable)
	void Server_AddToPendingSaves();
};
