// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "SaveHandler.h"
#include "Components/PlayerSaveHandlerComponent.h"
#include "WorldGenerationHandler.h"
#include "TimeOfDayHandler.h"
#include "Interfaces/RequiredForLoad.h"
#include "WeatherHandler.h"
#include "AnimalSpawnHandler.h"
#include "MonsterSpawnHandler.h"
#include "GameChatHandler.h"
#include "WildOmissionCore/WildOmissionGameInstance.h"
#include "Interfaces/OnlineFriendsInterface.h" 
#include "WildOmissionCore/WildOmissionGameState.h"
#include "WildOmissionCore/PlayerControllers/WildOmissionPlayerController.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"
#include "Components/PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Actors/Storm.h"

void AWildOmissionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SaveFile = UGameplayStatics::ParseOption(Options, "SaveGame");
	FriendsOnly = UGameplayStatics::ParseOption(Options, "FriendsOnly") == TEXT("1");
	
	SaveHandler = GetWorld()->SpawnActor<ASaveHandler>();
	WorldGenerationHandler = GetWorld()->SpawnActor<AWorldGenerationHandler>();
	TimeOfDayHandler = GetWorld()->SpawnActor<ATimeOfDayHandler>();
	WeatherHandler = GetWorld()->SpawnActor<AWeatherHandler>();
	AnimalSpawnHandler = GetWorld()->SpawnActor<AAnimalSpawnHandler>();
	MonsterSpawnHandler = GetWorld()->SpawnActor<AMonsterSpawnHandler>();
	ChatHandler = GetWorld()->SpawnActor<AGameChatHandler>();
	
	if (SaveHandler == nullptr)
	{
		return;
	}

	SaveHandler->SetGameSaveLoadController(Cast<IGameSaveLoadController>(GetGameInstance()));
	SaveHandler->SetSaveFile(SaveFile);
}

void AWildOmissionGameMode::StartPlay()
{
	Super::StartPlay();

	SaveHandler->LoadWorld();
}

void AWildOmissionGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UWildOmissionGameInstance* GameInstance = UWildOmissionGameInstance::GetWildOmissionGameInstance(GetWorld());
	if (FriendsOnly && !GameInstance->GetFriendsInterface()->IsFriend(0, *UniqueId.GetUniqueNetId().Get(), FString()))
	{
		ErrorMessage = TEXT("Player not friend, revoking connection attempt.");
	}

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
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

	APlayerState* NewPlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (ChatHandler == nullptr || NewPlayerState == nullptr)
	{
		return;
	}

	ChatHandler->SendMessage(NewPlayerState, TEXT("Has Joined The Game."), true);
}

void AWildOmissionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (GetWorld()->IsPlayInEditor())
	{
		return;
	}

	APlayerState* ExitingPlayerState = Exiting->GetPlayerState<APlayerState>();
	if (ChatHandler == nullptr || ExitingPlayerState == nullptr)
	{
		return;
	}

	ChatHandler->SendMessage(ExitingPlayerState, TEXT("Has Left The Game."), true);
}

void AWildOmissionGameMode::SpawnHumanForController(APlayerController* Controller)
{
	if (Controller == nullptr || !IsValid(Controller))
	{
		UE_LOG(LogGameMode, Warning, TEXT("Failed to spawn Human, PlayerController wasn't valid."));
		return;
	}

	AActor* StartSpot = FindPlayerStart(Controller);
	if (StartSpot == nullptr && Controller->StartSpot != nullptr)
	{
		StartSpot = Controller->StartSpot.Get();
	}
	
	if (StartSpot == nullptr)
	{
		return;
	}

	FRotator SpawnRotation = StartSpot->GetActorRotation();

	UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart %s"), (Controller && Controller->PlayerState) ? *Controller->PlayerState->GetPlayerName() : TEXT("Unknown"));

	if (MustSpectate(Cast<APlayerController>(Controller)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart: Tried to restart a spectator-only player!"));
		return;
	}

	if (HumanCharacterClass != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(HumanCharacterClass, StartSpot->GetActorLocation(), SpawnRotation);
		if (IsValid(NewPawn))
		{
			Controller->SetPawn(NewPawn);
		}
	}

	if (!IsValid(Controller->GetPawn()))
	{
		FailedToRestartPlayer(Controller);
	}
	else
	{
		// Tell the start spot it was used
		InitStartSpot(StartSpot, Controller);

		FinishRestartPlayer(Controller, SpawnRotation);
	}
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
		if (PlayerController == nullptr || PlayerController->GetPawn() == nullptr)
		{
			return;
		}

		FHitResult HitResult;
		const FVector Start = FVector(0.0f, 0.0f, 5000.0f);
		const FVector End = -(Start * 2.0f);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
		{
			PlayerController->GetPawn()->SetActorLocation(HitResult.ImpactPoint);
		}
	}
}

void AWildOmissionGameMode::SetTime(float NormalizedTime)
{
	if (TimeOfDayHandler == nullptr)
	{
		return;
	}

	TimeOfDayHandler->SetNormalizedProgressThroughDay(NormalizedTime);
}

void AWildOmissionGameMode::Weather(const FString& WeatherToSet)
{
	if (WeatherHandler == nullptr)
	{
		return;
	}

	AStorm* SpawnedStorm = WeatherHandler->SpawnStorm(true);
	
	if (WeatherToSet == TEXT("Rain"))
	{
		FVector NewStormLocation = SpawnedStorm->GetActorLocation();
		NewStormLocation.X = 0.0f;
		NewStormLocation.Y = 0.0f;
		SpawnedStorm->SetActorLocation(NewStormLocation);
		SpawnedStorm->SetSeverity(30.0f);
	}
	else if (WeatherToSet == TEXT("Tornado"))
	{
		FVector NewStormLocation = SpawnedStorm->GetActorLocation();
		NewStormLocation.X = 0.0f;
		NewStormLocation.Y = 0.0f;
		SpawnedStorm->SetActorLocation(NewStormLocation);
		SpawnedStorm->SetSeverity(90.0f);
	}
	else if (WeatherToSet == TEXT("Clear"))
	{
		WeatherHandler->ClearStorm();
	}
}

ASaveHandler* AWildOmissionGameMode::GetSaveHandler() const
{
	return SaveHandler;
}

AWorldGenerationHandler* AWildOmissionGameMode::GetWorldGenerationHandler() const
{
	return WorldGenerationHandler;
}

AWeatherHandler* AWildOmissionGameMode::GetWeatherHandler() const
{
	return WeatherHandler;
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