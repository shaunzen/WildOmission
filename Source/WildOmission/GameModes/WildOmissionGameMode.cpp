// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "WildOmission/Core/SaveSystem/SaveHandler.h"
#include "WildOmission/Core/SaveSystem/PlayerSaveHandlerComponent.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "WildOmission/Player/WildOmissionCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "WildOmission/Components/InventoryComponent.h"

void AWildOmissionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SaveGame = UGameplayStatics::ParseOption(Options, "SaveGame");

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

ASaveHandler* AWildOmissionGameMode::GetSaveHandler()
{
	return SaveHandler;
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
		
		AWildOmissionCharacter* Character = Cast<AWildOmissionCharacter>(PlayerController->GetPawn());
		if (Character == nullptr)
		{
			return;
		}
		for (const FInventoryItem& ItemData : Character->GetInventoryComponent()->GetContents()->Contents)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Orange, FString::Printf(TEXT("%s: %i"), *ItemData.Name.ToString(), ItemData.Quantity));
		}
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Green, FString::Printf(TEXT("Player: "), *Character->GetActorNameOrLabel()));
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