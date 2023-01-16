// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "WildOmission/Characters/PlayerCharacter.h"
#include "WildOmission/ActorComponents/InventoryComponent.h"
#include "WildOmission/SaveGames/WildOmissionSaveGame.h"
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
	WildOmissionSaveGame->PlayerPosition = GetPlayerLocation();

	// Save the data
	UGameplayStatics::SaveGameToSlot(WildOmissionSaveGame, FString("Save01"), 0);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Green, FString("Saved game"));
}

void AWildOmissionGameMode::LoadGame()
{
	WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
	if ((WildOmissionSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(FString("Save01"), 0))))
	{
		SetPlayerLocation(WildOmissionSaveGame->PlayerPosition);
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

FVector AWildOmissionGameMode::GetPlayerLocation()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return FVector::ZeroVector;
	}

	return PlayerController->GetPawn()->GetActorLocation();
}

void AWildOmissionGameMode::SetPlayerLocation(FVector InLocation)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	
	PlayerController->GetPawn()->SetActorLocation(InLocation);
}