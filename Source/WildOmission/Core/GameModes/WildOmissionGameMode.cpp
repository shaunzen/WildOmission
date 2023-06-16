// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "WildOmission/Core/SaveSystem/SaveHandler.h"
#include "WildOmission/Weather/WeatherManager.h"
#include "WildOmission/Core/SaveSystem/PlayerSaveHandlerComponent.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "WildOmission/Core/WildOmissionGameState.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "WildOmission/Weather/Storm.h"
#include "WildOmission/Core/SaveSystem/WorldGenerationHandlerComponent.h"

void AWildOmissionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SaveFile = UGameplayStatics::ParseOption(Options, "SaveGame");

	SaveHandler = GetWorld()->SpawnActor<ASaveHandler>();
	WeatherManager = GetWorld()->SpawnActor<AWeatherManager>();

	if (SaveHandler == nullptr)
	{
		return;
	}

	SaveHandler->SetSaveFile(SaveFile);
}

void AWildOmissionGameMode::StartPlay()
{
	Super::StartPlay();

	SaveHandler->LoadWorld();
}

void AWildOmissionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	AWildOmissionPlayerController* NewWildOmissionPlayer = Cast<AWildOmissionPlayerController>(NewPlayer);
	if (NewWildOmissionPlayer == nullptr)
	{
		return;
	}

	NewWildOmissionPlayer->Client_SetNumRequiredActors(IRequiredForLoad::GetNumRequiredActorsInWorld(GetWorld()));

	SaveHandler->GetPlayerHandler()->Load(NewPlayer);

	if (GetWorld()->IsPlayInEditor())
	{
		return;
	}

	AWildOmissionGameState* WOGameState = GetGameState<AWildOmissionGameState>();
	if (WOGameState == nullptr)
	{
		return;
	}

	APlayerState* NewPlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (NewPlayerState == nullptr)
	{
		return;
	}

	WOGameState->AddChatMessage(NewPlayerState, FString("Has Joined The Game."), true);
}

void AWildOmissionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	UE_LOG(LogTemp, Warning, TEXT("Player Has Logout."));
	if (GetWorld()->IsPlayInEditor())
	{
		return;
	}

	AWildOmissionGameState* WOGameState = GetGameState<AWildOmissionGameState>();
	if (WOGameState == nullptr)
	{
		return;
	}

	APlayerState* ExitingPlayerState = Exiting->GetPlayerState<APlayerState>();
	if (ExitingPlayerState == nullptr)
	{
		return;
	}

	WOGameState->AddChatMessage(ExitingPlayerState, FString("Has Left The Game."), true);
}

void AWildOmissionGameMode::SaveGame()
{
	SaveHandler->SaveGame();
}

void AWildOmissionGameMode::ResetLocationOfAllConnectedPlayers()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			return;
		}

		PlayerController->GetPawn()->SetActorLocation(FVector(0.0f, 0.0f, 1000.0f));
	}
}

void AWildOmissionGameMode::Weather(const FString& WeatherToSet)
{
	if (WeatherManager == nullptr)
	{
		return;
	}

	AStorm* SpawnedStorm = WeatherManager->SpawnStorm(true);
	
	if (WeatherToSet == FString("Rain"))
	{
		FVector NewStormLocation = SpawnedStorm->GetActorLocation();
		NewStormLocation.X = 0.0f;
		NewStormLocation.Y = 0.0f;
		SpawnedStorm->SetActorLocation(NewStormLocation);
		SpawnedStorm->SetSeverity(30.0f);
	}
	else if (WeatherToSet == FString("Tornado"))
	{
		FVector NewStormLocation = SpawnedStorm->GetActorLocation();
		NewStormLocation.X = 0.0f;
		NewStormLocation.Y = 0.0f;
		SpawnedStorm->SetActorLocation(NewStormLocation);
		SpawnedStorm->SetSeverity(90.0f);
	}
	else if (WeatherToSet == FString("Clear"))
	{
		WeatherManager->ClearStorm();
	}
}

ASaveHandler* AWildOmissionGameMode::GetSaveHandler() const
{
	return SaveHandler;
}

AWeatherManager* AWildOmissionGameMode::GetWeatherManager() const
{
	return WeatherManager;
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

void AWildOmissionGameMode::LogPlayerInventorySlots()
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
		for (const FInventorySlot& Slot : Character->GetInventoryComponent()->GetSlots())
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Orange, FString::Printf(TEXT("Index: %i, Item: %s, Quantity: %i"), Slot.Index, *Slot.Item.Name.ToString(), Slot.Item.Quantity));
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