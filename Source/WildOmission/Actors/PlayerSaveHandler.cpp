// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerSaveHandler.h"
#include "SaveHandler.h"
#include "WildOmission/PlayerControllers/WildOmissionPlayerController.h"
#include "WildOmission/SaveGames/WildOmissionSaveGame.h"
#include "WildOmission/GameModes/WildOmissionGameMode.h"

// Sets default values
APlayerSaveHandler::APlayerSaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerSaveHandler::BeginPlay()
{
	Super::BeginPlay();

	// TODO timer save all players
	
}

// Called every frame
void APlayerSaveHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerSaveHandler::SavePendingPlayers(TArray<FWildOmissionPlayerSave>& OutPlayerSaves)
{
	for (const FWildOmissionPlayerSave& PendingPlayerSave : PendingPlayerSaves)
	{
		int32 SaveIndex = 0;
		if (GetPlayerSaveIndex(PendingPlayerSave.UniqueID, SaveIndex))
		{
			OutPlayerSaves[SaveIndex] = PendingPlayerSave;
		}
		else
		{
			OutPlayerSaves.Add(PendingPlayerSave);
		}
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Cyan, FString::Printf(TEXT("Saved %i players from pending list and clearing the list"), PendingPlayerSaves.Num()));
	PendingPlayerSaves.Empty();
}

void APlayerSaveHandler::Setup(ASaveHandler* InOwnerSaveHandler)
{
	OwnerSaveHandler = InOwnerSaveHandler;
}

void APlayerSaveHandler::GatherAllPlayerSaves()
{
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Cyan, FString("Gathering all player data for pending list"));
	GatherSaveDataFromConnectedPlayers(GameMode->GetAllPlayerControllers(), PendingPlayerSaves);
}

void APlayerSaveHandler::GatherPlayerSave(APlayerController* PlayerControllerToSave)
{
	GatherSaveDataFromConnectedPlayer(PlayerControllerToSave, PendingPlayerSaves);
}

void APlayerSaveHandler::GatherSaveDataFromConnectedPlayers(TArray<APlayerController*> PlayerControllersToSave, TArray<FWildOmissionPlayerSave>& OutPendingSaves)
{
	for (APlayerController* PlayerController : PlayerControllersToSave)
	{
		GatherSaveDataFromConnectedPlayer(PlayerController, OutPendingSaves);
	}
}

void APlayerSaveHandler::GatherSaveDataFromConnectedPlayer(APlayerController* PlayerControllerToSave, TArray<FWildOmissionPlayerSave>& OutPendingSaves)
{
	AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerControllerToSave);
	if (WildOmissionPlayerController == nullptr)
	{
		return;
	}

	int32 SaveIndex = 0;
	if (GetPlayerPendingIndex(WildOmissionPlayerController->GetUniqueID(), SaveIndex))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Cyan, FString::Printf(TEXT("Found Playing already in pending list at index %i"), SaveIndex));
		OutPendingSaves[SaveIndex] = WildOmissionPlayerController->SavePlayer();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Cyan, FString("Player wasn't in the pending list appending to the end"));
		OutPendingSaves.Add(WildOmissionPlayerController->SavePlayer());
	}
}

// Finds if player ID is in pending list
bool APlayerSaveHandler::GetPlayerPendingIndex(FString PlayerUniqueID, int32& OutIndex)
{
	int32 Index = 0;
	bool PlayerFound = false;
	for (const FWildOmissionPlayerSave& PendingPlayerSave : PendingPlayerSaves)
	{
		if (PendingPlayerSave.UniqueID != PlayerUniqueID)
		{
			++Index;
			continue;
		}
		PlayerFound = true;
		break;
	}
	OutIndex = Index;
	return PlayerFound;
}

// Finds if player ID is in save file
bool APlayerSaveHandler::GetPlayerSaveIndex(FString PlayerUniqueID, int32& OutIndex)
{
	UWildOmissionSaveGame* SaveFile = OwnerSaveHandler->GetSaveFile();
	if (SaveFile == nullptr)
	{
		return false;
	}

	int32 Index = 0;
	bool PlayerFound = false;
	for (const FWildOmissionPlayerSave& PlayerSave : SaveFile->PlayerSaves)
	{
		if (PlayerSave.UniqueID != PlayerUniqueID)
		{
			++Index;
			continue;
		}

		PlayerFound = true;
		break;
	}

	OutIndex = Index;
	return PlayerFound;
}
