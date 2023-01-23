// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "WildOmission/Actors/SaveHandler.h"
#include "WildOmission/Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Actors/PlayerSaveHandler.h"
#include "GameFramework/PlayerState.h"
#include "WildOmission/ActorComponents/InventoryComponent.h"

void AWildOmissionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SaveGame = UGameplayStatics::ParseOption(Options, "SaveGame");

	if (SaveGame.Len() == 0)
	{
		return;
	}
	
	SaveHandler = GetWorld()->SpawnActor<ASaveHandler>();
	
	if (SaveHandler == nullptr)
	{
		return;
	}
	
	SaveHandler->LoadGame(SaveGame);
}

void AWildOmissionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	SaveHandler->GetPlayerHandler()->LoadPlayer(NewPlayer);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, FString::Printf(TEXT("%s Has Joined the game"), *NewPlayer->GetPlayerState<APlayerState>()->GetUniqueId().ToString()));
}

void AWildOmissionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AWildOmissionGameMode::SaveGame()
{
	SaveHandler->SaveGame();
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

TArray<APlayerController*> AWildOmissionGameMode::GetAllPlayerControllers()
{
	TArray<APlayerController*> PlayerControllers;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		PlayerControllers.Add(Iterator->Get());
	}

	return PlayerControllers;
}