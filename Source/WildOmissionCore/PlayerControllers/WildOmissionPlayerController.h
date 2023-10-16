// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/SavablePlayer.h"
#include "Interfaces/PlayerControllerMessageSender.h"
#include "Interfaces/BedController.h"
#include "WildOmissionPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishedLoadingSignature, AWildOmissionPlayerController*, LoadedController);

class UMusicPlayerComponent;
class UDeathMenuWidget;

UCLASS()
class WILDOMISSIONCORE_API AWildOmissionPlayerController : public APlayerController, public ISavablePlayer, public IPlayerControllerMessageSender, public IBedController
{
	GENERATED_BODY()

public:
	AWildOmissionPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Begin ISavablePlayer Implementation
	virtual FPlayerSave SavePlayer() override;
	virtual void LoadPlayerSave(const FPlayerSave& PlayerSave) override;
	virtual FString GetUniqueID() override;
	virtual bool IsHost() override;
	// End ISavablePlayer Implementation

	// Begin IPlayerControllerMessageSender Implementation
	virtual void SendMessage(APlayerState* Sender, const FString& Message) override;
	// End IPlayerControllerMessageSender Implementation

	// Begin IBedController Implementation
	virtual void SetBedUniqueID(const int32& InUniqueID) override;
	virtual int32 GetBedUniqueID() const override;
	// End IBedController Implementation

	void Save();

	UFUNCTION(BlueprintCallable)
	bool IsEditorPlayer() const;

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
	virtual void OnPossess(APawn* aPawn) override;

private:
	UPROPERTY(VisibleAnywhere)
	UMusicPlayerComponent* MusicPlayerComponent;

	UPROPERTY()
	TSubclassOf<UDeathMenuWidget> DeathMenuWidgetClass;

	UPROPERTY(Replicated)
	int32 BedUniqueID;

	int32 NumRequiredActorsForLoad;
	FTimerHandle ValidateWorldStateTimerHandle;
	FTimerHandle UpdateClientRequiredActorCountTimerHandle;
	void ValidateWorldState();

	UFUNCTION()
	void CountRequiredActorsAndSendToClient();

	void StopLoading();

	// Used by the server to figure out if a player is still loading
	// into the server, it will wait until the player if fully loaded before attempting
	// to load their data from the save file.
	bool bIsStillLoading;
	FPlayerSave StoredPlayerSave;

	UFUNCTION(Server, Reliable)
	void Server_SendMessage(APlayerState* Sender, const FString& Message);

	UFUNCTION(Server, Reliable)
	void Server_AddToPendingSaves();

	UFUNCTION(Server, Reliable)
	void Server_KillThisPlayer();

};
