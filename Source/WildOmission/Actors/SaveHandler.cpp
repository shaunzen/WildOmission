// Copyright Telephone Studios. All Rights Reserved.


#include "SaveHandler.h"
#include "WildOmission/PlayerControllers/WildOmissionPlayerController.h"
#include "WildOmission/GameModes/WildOmissionGameMode.h"
#include "WildOmission/SaveGames/WildOmissionSaveGame.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASaveHandler::ASaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASaveHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASaveHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaveHandler::SaveGame()
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Aborting save, SaveFile was nullptr."));
		return;
	}

	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Aborting save, GameMode returned a nullptr."));
		return;
	}

	CreatePlayerSaves(GameMode->GetAllPlayerControllers(), SaveFile->PlayerSaves);

	UpdateSaveFile(SaveFile);
}

void ASaveHandler::CreatePlayerSaves(TArray<APlayerController*> PlayerControllersToSave, TArray<FWildOmissionPlayerSave>& OutPlayerSaves)
{
	OutPlayerSaves.Empty();
	for (APlayerController* PlayerController : PlayerControllersToSave)
	{
		AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);

		OutPlayerSaves.Add(WildOmissionPlayerController->SavePlayer());
	}
}

void ASaveHandler::LoadGame(const FString& SaveFileName)
{
	CurrentSaveFileName = SaveFileName;

	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Save File Returned a nullptr... Aborting load"));
		return;
	}

	if (SaveFile->CreationInformation.LevelHasGenerated == false)
	{
		// TODO generate level
	}

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, CurrentSaveFileName);
}

void ASaveHandler::LoadPlayer(APlayerController* PlayerController)
{
	FString PlayerUniqueID = PlayerController->GetPlayerState<APlayerState>()->GetUniqueId().ToString();
	FWildOmissionPlayerSave PlayerSave;
	if (!RetrivePlayerDataFromSave(PlayerUniqueID, PlayerSave))
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString("Player hasn't played on this save file before generating new information."));
		// Generate new player
	}
	AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);
	if (WildOmissionPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load player, couldn't cast to AWildOmissionPlayerController."));
		return;
	}

	WildOmissionPlayerController->LoadPlayerSave(PlayerSave);
}

bool ASaveHandler::RetrivePlayerDataFromSave(FString PlayerUniqueID, FWildOmissionPlayerSave& OutPlayerSave)
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveFile was nullptr aborting player load on player %s."), *PlayerUniqueID);
		return false;
	}

	for (const FWildOmissionPlayerSave& PlayerSave : SaveFile->PlayerSaves)
	{
		if (PlayerSave.UniqueID != PlayerUniqueID)
		{
			continue;
		}

		OutPlayerSave = PlayerSave;
		return true;
	}

	return false;
}

UWildOmissionSaveGame* ASaveHandler::GetSaveFile()
{
	UWildOmissionSaveGame* SaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
	SaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveFileName, 0));

	return SaveFile;
}

void ASaveHandler::UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile)
{
	if (UpdatedSaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Aborting update to save file, updated save file passed in was a nullptr."));
		return;
	}

	UGameplayStatics::SaveGameToSlot(UpdatedSaveFile, CurrentSaveFileName, 0);
}