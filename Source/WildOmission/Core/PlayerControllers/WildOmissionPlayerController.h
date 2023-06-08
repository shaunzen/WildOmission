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

	UFUNCTION(Client, Reliable)
	void Client_ShowDeathMenu();

	FString GetUniqueID();

	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(APlayerState* Sender, const FString& Message);


	//*****************************
	// Console functions
	UFUNCTION(Exec)
	void Kill();

	UFUNCTION(Exec)
	void LogLocalInventoryContents();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TSubclassOf<UDeathMenuWidget> DeathMenuWidgetClass;

	UFUNCTION(Server, Reliable)
	void Server_AddToPendingSaves();
	
	UFUNCTION(Server, Reliable)
	void Server_KillThisPlayer();

};
