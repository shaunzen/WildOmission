// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/SavablePlayer.h"
#include "Interfaces/ChatMessageSender.h"
#include "WildOmissionPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishedLoadingSignature, AWildOmissionPlayerController*, LoadedController);

class UInventoryComponent;
class UDeathMenuWidget;

UCLASS()
class WILDOMISSION_API AWildOmissionPlayerController : public APlayerController, public ISavablePlayer, public IChatMessageSender
{
	GENERATED_BODY()

public:
	AWildOmissionPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	// Begin ISavablePlayer Implementation
	virtual FPlayerSave SavePlayer() override;
	virtual void LoadPlayerSave(const FPlayerSave& PlayerSave) override;
	virtual FString GetUniqueID() override;
	// End ISavablePlayer Implementation

	// Begin IChatMessageSender Implementation
	virtual void SendMessage(APlayerState* Sender, const FString& Message) override;
	// End IChatMessageSender Implementation

	void Save();

	UFUNCTION(Client, Reliable)
	void Client_ShowDeathMenu();

	UFUNCTION(Client, Reliable)
	void Client_SetNumRequiredActors(const int32& InNum);

	UFUNCTION(Server, Reliable)
	void Server_Spawn();

	FOnFinishedLoadingSignature OnFinishedLoading;

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

	int32 NumRequiredActorsForLoad;
	FTimerHandle ValidateWorldStateTimerHandle;
	FTimerHandle UpdateClientRequiredActorCountTimerHandle;
	void ValidateWorldState();

	void StopLoading();

	// Used by the server to figure out if a player is still loading
	// into the server, it will wait until the player if fully loaded before attempting
	// to load their data from the save file.
	bool bIsStillLoading;

	UFUNCTION(Server, Reliable)
	void Server_AddToPendingSaves();

	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(APlayerState* Sender, const FString& Message);

	UFUNCTION(Server, Reliable)
	void Server_KillThisPlayer();

};
