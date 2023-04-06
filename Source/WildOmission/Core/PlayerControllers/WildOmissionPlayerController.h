// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WildOmissionPlayerController.generated.h"

class UInventoryComponent;
class UDeathMenuWidget;

UCLASS()
class WILDOMISSION_API AWildOmissionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWildOmissionPlayerController();

	struct FWildOmissionPlayerSave SavePlayer();

	void LoadPlayerSave(const struct FWildOmissionPlayerSave& PlayerSave);

	void Save();
	
	UFUNCTION(Server, Reliable)
	void Server_RequestRespawn();

	UFUNCTION(Client, Reliable)
	void Client_ShowDeathMenu();

	FString GetUniqueID();

	//*****************************
	// Console functions
	UFUNCTION(Exec)
	void Kill();
	UFUNCTION(Exec)
	void LogLocalInventoryContents();
	
protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	TSubclassOf<UDeathMenuWidget> DeathMenuWidgetClass;

	UFUNCTION(Server, Reliable)
	void Server_AddToPendingSaves();
	
	UFUNCTION(Server, Reliable)
	void Server_KillThisPlayer();

};
