// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "WildOmission/Characters/PlayerCharacter.h"
#include "WildOmission/ActorComponents/InventoryComponent.h"
#include "WildOmission/SaveGames/WildOmissionSaveGame.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void AWildOmissionGameMode::SaveGame()
{
	// Find existing save or create a new one
	WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(FString("Save01"), 0));
	if (WildOmissionSaveGame == nullptr)
	{
		// Failed to find existing save creating a new one
		WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
		if (WildOmissionSaveGame == nullptr)
		{
			return;
		}
	}
	
	// Fill the save data
	SavePlayers(WildOmissionSaveGame->PlayerSaves);

	// Save the data
	UGameplayStatics::SaveGameToSlot(WildOmissionSaveGame, FString("Save01"), 0);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Green, FString("Saved game"));
}

void AWildOmissionGameMode::LoadGame()
{
	WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
	if ((WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(FString("Save01"), 0))))
	{
		LoadPlayers(WildOmissionSaveGame->PlayerSaves);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Green, FString("Loaded game"));
	}
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

void AWildOmissionGameMode::SavePlayers(TArray<FWildOmissionPlayerSave>& OutPlayerSaves)
{
	// Clear it out, later we need to overwrite elements with new info and keep old ones
	OutPlayerSaves.Empty();
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

		FWildOmissionPlayerSave NewPlayer;
		NewPlayer.ID = PlayerState->GetPlayerId();
		NewPlayer.WorldLocation = PlayerController->GetPawn()->GetActorLocation();

		OutPlayerSaves.Add(NewPlayer);
	}
}

void AWildOmissionGameMode::LoadPlayers(const TArray<FWildOmissionPlayerSave>& PlayerSaves)
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
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Red, FString::Printf(TEXT("Player save size: %i"), PlayerSaves.Num()));
		for (const FWildOmissionPlayerSave& PlayerSave : PlayerSaves)
		{

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Blue, FString::Printf(TEXT("Found player save with matching id of: %i"), PlayerSave.ID));
			if (PlayerSave.ID != PlayerState->GetPlayerId())
			{
				continue;
			}
			PlayerController->GetPawn()->SetActorLocation(PlayerSave.WorldLocation);
		}
	}
}

void AWildOmissionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	int32 NewID = FMath::CeilToInt32(GetWorld()->GetDeltaSeconds() * GetWorld()->GetDeltaSeconds());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, FString::Printf(TEXT("ID: %i"), NewID));
	NewPlayer->GetPlayerState<APlayerState>()->SetPlayerId(NewID);
}