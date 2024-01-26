// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "SaveManager.h"
#include "Components/PlayerSaveManagerComponent.h"
#include "Deployables/Bed.h"
#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "TimeOfDayManager.h"
#include "WeatherManager.h"
#include "AnimalSpawnManager.h"
#include "MonsterSpawnManager.h"
#include "GameChatManager.h"
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
	
	SaveManager = GetWorld()->SpawnActor<ASaveManager>();
	ChunkManager = GetWorld()->SpawnActor<AChunkManager>();
	TimeOfDayManager = GetWorld()->SpawnActor<ATimeOfDayManager>();
	WeatherManager = GetWorld()->SpawnActor<AWeatherManager>();
	AnimalSpawnManager = GetWorld()->SpawnActor<AAnimalSpawnManager>();
	MonsterSpawnManager = GetWorld()->SpawnActor<AMonsterSpawnManager>();
	ChatManager = GetWorld()->SpawnActor<AGameChatManager>();
	
	if (SaveManager == nullptr)
	{
		return;
	}

	SaveManager->SetGameSaveLoadController(Cast<IGameSaveLoadController>(GetGameInstance()));
	SaveManager->SetSaveFile(SaveFile);
}

void AWildOmissionGameMode::StartPlay()
{
	Super::StartPlay();

	SaveManager->LoadWorld();
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

	SaveManager->GetPlayerManager()->Load(NewPlayer);

	if (GetWorld()->IsPlayInEditor())
	{
		return;
	}

	APlayerState* NewPlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (ChatManager == nullptr || NewPlayerState == nullptr)
	{
		return;
	}

	ChatManager->SendMessage(NewPlayerState, TEXT("Has Joined The Game."), true);
}

void AWildOmissionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (GetWorld()->IsPlayInEditor())
	{
		return;
	}

	APlayerState* ExitingPlayerState = Exiting->GetPlayerState<APlayerState>();
	if (ChatManager == nullptr || ExitingPlayerState == nullptr)
	{
		return;
	}

	ChatManager->SendMessage(ExitingPlayerState, TEXT("Has Left The Game."), true);
}

void AWildOmissionGameMode::SpawnHumanForController(APlayerController* Controller)
{
	if (Controller == nullptr || !IsValid(Controller))
	{
		UE_LOG(LogGameMode, Warning, TEXT("Failed to spawn Human, PlayerController wasn't valid."));
		return;
	}

	if (APawn* Pawn = Controller->GetPawnOrSpectator())
	{
		Controller->UnPossess();
		Pawn->Destroy();
	}

	AWildOmissionPlayerController* WOPlayerController = Cast<AWildOmissionPlayerController>(Controller);
	if (WOPlayerController && WOPlayerController->GetBedUniqueID() != -1)
	{
		SpawnHumanAtBed(Controller);
	}
	else
	{
		SpawnHumanAtStartSpot(Controller);
	}
}

void AWildOmissionGameMode::SaveGame()
{
	SaveManager->SaveGame();
}


void AWildOmissionGameMode::TeleportAllPlayersToSelf()
{
	APlayerController* SelfPlayerController = GetWorld()->GetFirstPlayerController();
	if (SelfPlayerController == nullptr)
	{
		return;
	}

	APawn* SelfPlayerPawn = SelfPlayerController->GetPawn();
	if (SelfPlayerPawn == nullptr)
	{
		return;
	}
	
	const FVector TargetLocation = SelfPlayerPawn->GetActorLocation();

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			continue;
		}

		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn == nullptr || PlayerPawn == SelfPlayerPawn)
		{
			continue;
		}
	
		PlayerPawn->SetActorLocation(TargetLocation);
	}
}

void AWildOmissionGameMode::GiveSelfGodMode()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	APlayerController* SelfPlayerController = World->GetFirstPlayerController();
	if (SelfPlayerController == nullptr)
	{
		return;
	}

	AWildOmissionCharacter* SelfCharacter = Cast<AWildOmissionCharacter>(SelfPlayerController->GetPawn());
	if (SelfCharacter == nullptr)
	{
		return;
	}

	SelfCharacter->SetGodMode(true);
}

void AWildOmissionGameMode::GiveAllPlayersGodMode()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			return;
		}

		AWildOmissionCharacter* PlayerCharacter = Cast<AWildOmissionCharacter>(PlayerController->GetPawn());
		if (PlayerCharacter == nullptr)
		{
			return;
		}

		PlayerCharacter->SetGodMode(true);
	}
}

void AWildOmissionGameMode::RemoveSelfGodMode()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	APlayerController* SelfPlayerController = World->GetFirstPlayerController();
	if (SelfPlayerController == nullptr)
	{
		return;
	}

	AWildOmissionCharacter* SelfCharacter = Cast<AWildOmissionCharacter>(SelfPlayerController->GetPawn());
	if (SelfCharacter == nullptr)
	{
		return;
	}

	SelfCharacter->SetGodMode(false);
}

void AWildOmissionGameMode::RemoveAllPlayersGodMode()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			return;
		}

		AWildOmissionCharacter* PlayerCharacter = Cast<AWildOmissionCharacter>(PlayerController->GetPawn());
		if (PlayerCharacter == nullptr)
		{
			return;
		}

		PlayerCharacter->SetGodMode(false);
	}
}

void AWildOmissionGameMode::FreezeTime()
{
	if (TimeOfDayManager == nullptr)
	{
		return;
	}

	TimeOfDayManager->SetTimeFrozen(true);
}

void AWildOmissionGameMode::UnfreezeTime()
{
	if (TimeOfDayManager == nullptr)
	{
		return;
	}

	TimeOfDayManager->SetTimeFrozen(false);
}

void AWildOmissionGameMode::SetTime(float NormalizedTime)
{
	if (TimeOfDayManager == nullptr)
	{
		return;
	}

	TimeOfDayManager->SetNormalizedProgressThroughDay(NormalizedTime);
}

void AWildOmissionGameMode::Weather(const FString& WeatherToSet)
{
	if (WeatherManager == nullptr)
	{
		return;
	}

	AStorm* SpawnedStorm = WeatherManager->SpawnStorm(true);
	
	APawn* HostPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (WeatherToSet == TEXT("Rain"))
	{
		FVector NewStormLocation = SpawnedStorm->GetActorLocation();
		NewStormLocation.X = HostPlayer->GetActorLocation().X - 10000.0f;
		NewStormLocation.Y = HostPlayer->GetActorLocation().Y;
		SpawnedStorm->SetActorLocation(NewStormLocation);
		SpawnedStorm->SetMovementVector(FVector(1.0f, 0.0f, 0.0f));
		SpawnedStorm->SetSeverity(30.0f);
	}
	else if (WeatherToSet == TEXT("Tornado"))
	{
		FVector NewStormLocation = SpawnedStorm->GetActorLocation();
		NewStormLocation.X = HostPlayer->GetActorLocation().X - 10000.0f;
		NewStormLocation.Y = HostPlayer->GetActorLocation().Y;
		SpawnedStorm->SetActorLocation(NewStormLocation);
		SpawnedStorm->SetMovementVector(FVector(1.0f, 0.0f, 0.0f));
		SpawnedStorm->SetSeverity(90.0f);
	}
	else if (WeatherToSet == TEXT("Clear"))
	{
		WeatherManager->ClearStorm();
	}
}

ASaveManager* AWildOmissionGameMode::GetSaveManager() const
{
	return SaveManager;
}

AChunkManager* AWildOmissionGameMode::GetChunkManager() const
{
	return ChunkManager;
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

void AWildOmissionGameMode::SpawnHumanAtStartSpot(AController* Controller)
{
	if (ChunkManager == nullptr)
	{
		return;
	}

	const FVector SpawnLocation = ChunkManager->GetWorldSpawnPoint();

	// Spawn there
	if (MustSpectate(Cast<APlayerController>(Controller)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart: Tried to restart a spectator-only player!"));
		return;
	}

	if (HumanCharacterClass != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(HumanCharacterClass, SpawnLocation + FVector(0.0f, 0.0f, 100.0f), FRotator::ZeroRotator);
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
		FinishRestartPlayer(Controller, FRotator::ZeroRotator);
	}
}

void AWildOmissionGameMode::SpawnHumanAtBed(AController* Controller)
{
	AWildOmissionPlayerController* WOPlayerController = Cast<AWildOmissionPlayerController>(Controller);
	if (WOPlayerController == nullptr)
	{
		SpawnHumanAtStartSpot(Controller);
		return;
	}
	
	const float ChunkSize = AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale();
	const FVector BedWorldLocation = WOPlayerController->GetBedWorldLocation();
	const FIntVector2 BedChunkLocation(FMath::RoundToInt32(BedWorldLocation.X / ChunkSize), FMath::RoundToInt32(BedWorldLocation.Y / ChunkSize));

	AChunk* BedChunk = ChunkManager->GetChunkAtLocation(BedChunkLocation);
	if (BedChunk == nullptr)
	{
		return;
	}

	TArray<AActor*> ChunkAttachedActors;
	BedChunk->GetAttachedActors(ChunkAttachedActors);

	TArray<ABed*> ChunkBeds;
	for (AActor* AttachedActor : ChunkAttachedActors)
	{
		ABed* AttachedBed = Cast<ABed>(AttachedActor);
		if (AttachedBed == nullptr)
		{
			continue;
		}

		ChunkBeds.Add(AttachedBed);
	}

	ABed* SpawnBed = nullptr;
	for (ABed* Bed : ChunkBeds)
	{
		if (Bed == nullptr)
		{
			continue;
		}

		if (Bed->GetUniqueID() == WOPlayerController->GetBedUniqueID())
		{
			SpawnBed = Bed;
			break;
		}
	}

	if (SpawnBed == nullptr)
	{
		SpawnHumanAtStartSpot(Controller);
		return;
	}

	const FVector SpawnLocation = SpawnBed->GetSpawnPointComponent()->GetComponentLocation() + FVector(0.0f, 0.0f, 100.0f);
	const FRotator SpawnRotation = SpawnBed->GetSpawnPointComponent()->GetComponentRotation();

	if (HumanCharacterClass != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(HumanCharacterClass, SpawnLocation, SpawnRotation);
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
		FinishRestartPlayer(Controller, SpawnRotation);
	}
}
