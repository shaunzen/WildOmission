// Copyright Telephone Studios. All Rights Reserved.


#include "SaveHandler.h"
#include "PlayerSaveHandler.h"
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
	
	PlayerSaveHandler = GetWorld()->SpawnActor<APlayerSaveHandler>();
	PlayerSaveHandler->Setup(this);
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

	PlayerSaveHandler->SavePendingPlayers(SaveFile->PlayerSaves);
	
	UpdateSaveFile(SaveFile);
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
	AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);
	if (WildOmissionPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load player, couldn't cast to AWildOmissionPlayerController."));
		return;
	}
	
	FString PlayerUniqueID = PlayerController->GetPlayerState<APlayerState>()->GetUniqueId().ToString();
	FWildOmissionPlayerSave PlayerSave;
	if (!RetrivePlayerDataFromSave(PlayerUniqueID, PlayerSave) || PlayerSave.IsAlive == false)
	{
		WildOmissionPlayerController->Spawn();
	}


	WildOmissionPlayerController->LoadPlayerSave(PlayerSave);
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