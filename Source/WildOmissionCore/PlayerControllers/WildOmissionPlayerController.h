// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/SavablePlayer.h"
#include "Interfaces/PlayerControllerMessageSender.h"
#include "Interfaces/BedController.h"
#include "WildOmissionPlayerController.generated.h"

UCLASS()
class WILDOMISSIONCORE_API AWildOmissionPlayerController : public APlayerController, public ISavablePlayer, public IPlayerControllerMessageSender, public IBedController
{
	GENERATED_BODY()

public:
	AWildOmissionPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Begin ISavablePlayer Implementation
	virtual FPlayerSaveData SavePlayer() override;
	virtual void LoadPlayerSave(const FPlayerSaveData& SaveData) override;
	virtual bool IsStillLoading() const override;
	virtual FString GetUniqueID() const override;
	virtual bool IsHost() const override;
	// End ISavablePlayer Implementation

	// Begin IPlayerControllerMessageSender Implementation
	virtual void SendMessage(APlayerState* Sender, const FString& Message) override;
	// End IPlayerControllerMessageSender Implementation

	// Begin IBedController Implementation
	virtual void SetCurrentBed(const int32& InBedUniqueID, const FVector& InBedWorldLocation) override;
	virtual int32 GetBedUniqueID() const override;
	virtual FVector GetBedWorldLocation() const override;
	// End IBedController Implementation

	void Save();

	UFUNCTION(BlueprintCallable)
	bool IsEditorPlayer() const;

	UFUNCTION(Client, Reliable)
	void Client_ShowDeathMenu();

	UFUNCTION(Server, Reliable)
	void Server_Spawn();

	//*****************************
	// Console functions
	UFUNCTION(Exec)
	void Kill();

	// This isn't really used anymore
	UFUNCTION(Exec)
	void LogLocalInventoryContents();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;

private:
	UPROPERTY(VisibleAnywhere)
	class UMusicPlayerComponent* MusicPlayerComponent;

	UPROPERTY()
	TSubclassOf<class UDeathMenuWidget> DeathMenuWidgetClass;

	UPROPERTY(Replicated)
	int32 BedUniqueID;

	UPROPERTY()
	FVector BedWorldLocation;

	UPROPERTY(Replicated)
	FIntVector2 SpawnChunk;

	FTimerHandle CheckSpawnChunkValidTimerHandle;

	void SetupPlayerOnServer();
	void StartLoading();
	void SetupMusicPlayerComponent();
	void SetupLocalAsHost();
	void SetupLocalAsClient();

	UFUNCTION()
	void CheckSpawnChunkValid();
	
	void StopLoading();

	// Used by the server to figure out if a player is still loading
	// into the server, it will wait until the player if fully loaded before attempting
	// to load their data from the save file.
	bool bIsStillLoading;
	FPlayerSaveData StoredPlayerSaveData;

	UFUNCTION(Server, Reliable)
	void Server_SendMessage(APlayerState* Sender, const FString& Message);

	UFUNCTION(Server, Reliable)
	void Server_AddToPendingSaves();

	UFUNCTION(Server, Reliable)
	void Server_KillThisPlayer();

};
