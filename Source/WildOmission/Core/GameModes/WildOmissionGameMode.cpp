// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "WildOmission/Core/SaveSystem/SaveHandler.h"
#include "WildOmission/Core/SaveSystem/PlayerSaveHandlerComponent.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "WildOmission/Weather/Tornado.h"
#include "UObject/ConstructorHelpers.h"

AWildOmissionGameMode::AWildOmissionGameMode()
{
	ConstructorHelpers::FClassFinder<ATornado> TornadoBlueprint(TEXT("/Game/WildOmission/Weather/BP_Tornado"));
	if (TornadoBlueprint.Succeeded())
	{
		TornadoClass = TornadoBlueprint.Class;
	}
}

void AWildOmissionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SaveFile = UGameplayStatics::ParseOption(Options, "SaveGame");

	SaveHandler = GetWorld()->SpawnActor<ASaveHandler>();

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
	
	SaveHandler->GetPlayerHandler()->Load(NewPlayer);
}

void AWildOmissionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
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

void AWildOmissionGameMode::SpawnTornado()
{
	ATornado* SpawnedTornado = GetWorld()->SpawnActor<ATornado>(TornadoClass);
	SpawnedTornado->OnSpawn();
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