// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerSaveHandler.h"
#include "SaveHandler.h"
#include "WildOmission/SaveGames/WildOmissionSaveGame.h"
#include "WildOmission/PlayerControllers/WildOmissionPlayerController.h"
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
	
}

// Called every frame
void APlayerSaveHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerSaveHandler::Setup(ASaveHandler* InOwner)
{
	OwnerSaveHandler = InOwner;
}

void APlayerSaveHandler::SavePlayers(TArray<FWildOmissionPlayerSave>& OutUpdatedPlayerSaves)
{
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save players, GameMode was nullptr."));
		return;
	}

	CreatePlayerSaves(GameMode->GetAllPlayerControllers(), OutUpdatedPlayerSaves);
}

void APlayerSaveHandler::LoadPlayer(APlayerController* PlayerController)
{
	AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);
	if (WildOmissionPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load player, couldn't cast PlayerController to a WildOmissionPlayerController."));
		return;
	}

	FString PlayerUniqueID = WildOmissionPlayerController->GetUniqueID();
	FWildOmissionPlayerSave PlayerSave;
	if (!RetrivePlayerDataFromSave(PlayerUniqueID, PlayerSave) || PlayerSave.IsAlive == false)
	{
		WildOmissionPlayerController->Spawn();
		return;
	}

	WildOmissionPlayerController->LoadPlayerSave(PlayerSave);
}

void APlayerSaveHandler::CreatePlayerSaves(TArray<APlayerController*> PlayerControllersToSave, TArray<FWildOmissionPlayerSave>& OutPlayerSaves)
{
	for (APlayerController* PlayerController : PlayerControllersToSave)
	{
		AWildOmissionPlayerController* WildOmissionPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);
		if (WildOmissionPlayerController == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create player saves, could't cast PlayerController to a WildOmissionPlayerController."));
			return;
		}

		int32 SaveIndex = 0;
		if (GetPlayerSaveIndex(WildOmissionPlayerController->GetUniqueID(), SaveIndex))
		{
			OutPlayerSaves[SaveIndex] = WildOmissionPlayerController->SavePlayer();
		}
		else
		{
			OutPlayerSaves.Add(WildOmissionPlayerController->SavePlayer());
		}
	}
}

bool APlayerSaveHandler::RetrivePlayerDataFromSave(FString PlayerUniqueID, FWildOmissionPlayerSave& OutPlayerSave)
{
	UWildOmissionSaveGame* SaveFile = OwnerSaveHandler->GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrive player data from save, save file was a nullptr."));
		return false;
	}

	int32 PlayerSaveIndex = 0;
	if (!GetPlayerSaveIndex(PlayerUniqueID, PlayerSaveIndex))
	{
		return false;
	}

	OutPlayerSave = SaveFile->PlayerSaves[PlayerSaveIndex];
	return true;
}

bool APlayerSaveHandler::GetPlayerSaveIndex(FString PlayerUniqueID, int32& OutIndex)
{
	int32 Index = 0;
	bool PlayerFound = false;
	UWildOmissionSaveGame* SaveFile = OwnerSaveHandler->GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get player index, save file was nullptr."));
		return PlayerFound;
	}

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