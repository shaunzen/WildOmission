// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "WildOmission/Characters/PlayerCharacter.h"
#include "WildOmission/ActorComponents/InventoryComponent.h"
#include "WildOmission/SaveGames/WildOmissionSaveGame.h"
#include "WildOmission/PlayerControllers/WildOmissionPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void AWildOmissionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SaveGame = UGameplayStatics::ParseOption(Options, "SaveGame");

	if (SaveGame.Len() == 0)
	{
		return;
	}

	CurrentSaveName = SaveGame;
	LoadGame();
}

void AWildOmissionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// TODO use steam id instead

	int32 NewID = FMath::CeilToInt32(GetWorld()->UnpausedTimeSeconds);

	NewPlayer->GetPlayerState<APlayerState>()->SetPlayerId(NewID);

	LoadPlayer(NewPlayer->GetPlayerState<APlayerState>()->GetPlayerId(), NewPlayer);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, FString::Printf(TEXT("Given new player the id: %i"), NewID));
}

void AWildOmissionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	SaveGame();
}

void AWildOmissionGameMode::SaveGame()
{
	// Find existing save or create a new one
	WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveName, 0));
	if (WildOmissionSaveGame == nullptr)
	{
		// Failed to find existing save creating a new one
		WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
		if (WildOmissionSaveGame == nullptr)
		{
			return;
		}
	}
	
	// Populate the save data
	SavePlayers(WildOmissionSaveGame->PlayerSaves);

	// Save the data
	UGameplayStatics::SaveGameToSlot(WildOmissionSaveGame, CurrentSaveName, 0);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Green, FString("Saved game"));
}

void AWildOmissionGameMode::LoadGame()
{
	WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
	WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveName, 0));
	
	if (WildOmissionSaveGame == nullptr)
	{
		return;
	}

	if (WildOmissionSaveGame->CreationInformation.bLevelHasGenerated == false)
	{
		GenerateLevel();
	}

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Green, CurrentSaveName);
}

void AWildOmissionGameMode::LogPlayerInventoryComponents()
{
	// Get all the players in the current world
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			return;
		}
		
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
		if (PlayerCharacter == nullptr)
		{
			return;
		}
		for (const TPair<FName, int32>& Item : *PlayerCharacter->GetInventoryComponent()->GetContents())
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Orange, FString::Printf(TEXT("%s: %i"), *Item.Key.ToString(), Item.Value));
		}
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Green, FString::Printf(TEXT("Player: "), *PlayerCharacter->GetActorNameOrLabel()));
	}
}

void AWildOmissionGameMode::GenerateLevel()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("this save is brand new generating new data"));
	//TODO run world generation and things
}

void AWildOmissionGameMode::GeneratePlayer(APlayerController* PlayerController)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Player Hasnt joined on this save before generating new data"));
	// TODO find location in world for player to spawn
}

void AWildOmissionGameMode::SavePlayers(TArray<FWildOmissionPlayerSave>& OutPlayerSaves)
{
	// Clear it out, later we need to overwrite elements with new info and keep old ones
	OutPlayerSaves.Empty();
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AWildOmissionPlayerController* PlayerController = Cast<AWildOmissionPlayerController>(Iterator->Get());
		if (PlayerController == nullptr)
		{
			return;
		}
		// TODO if the id is already in in the list remove it and replace with the new one
		OutPlayerSaves.Add(PlayerController->SavePlayer());
	}
}

void AWildOmissionGameMode::LoadAllPlayers(const TArray<FWildOmissionPlayerSave>& PlayerSaves)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		
		if (PlayerController == nullptr)
		{
			return;
		}

		APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
		
		if (PlayerState == nullptr)
		{
			return;
		}
		
		LoadPlayer(PlayerState->GetPlayerId(), PlayerController);
	}
}

void AWildOmissionGameMode::LoadPlayer(int32 ID, APlayerController* PlayerController)
{
	AWildOmissionPlayerController* WOPlayerController = Cast<AWildOmissionPlayerController>(PlayerController);
	
	WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveName, 0));

	if (WildOmissionSaveGame == nullptr || WOPlayerController == nullptr)
	{
		return;
	}
	
	for (const FWildOmissionPlayerSave& PlayerSave : WildOmissionSaveGame->PlayerSaves)
	{
		if (PlayerSave.ID != WOPlayerController->GetPlayerState<APlayerState>()->GetPlayerId())
		{
			continue;
		}
		WOPlayerController->LoadPlayerSave(PlayerSave);
		return;
	}

	GeneratePlayer(WOPlayerController);
}