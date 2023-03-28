// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionPlayerController.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Components/VitalsComponent.h"
#include "GameFramework/PlayerState.h"
#include "WildOmission/Core/GameModes/WildOmissionGameMode.h"
#include "WildOmission/Core/SaveSystem/SaveHandler.h"
#include "WildOmission/Core/SaveSystem/PlayerSaveHandlerComponent.h"

FWildOmissionPlayerSave AWildOmissionPlayerController::SavePlayer()
{
	FWildOmissionPlayerSave PlayerSave;
	if (HasAuthority() == false)
	{
		return PlayerSave;
	}

	APlayerState* CurrentPlayerState = PlayerState.Get();
	if (CurrentPlayerState == nullptr)
	{
		return PlayerSave;
	}
	
	AWildOmissionCharacter* WildOmissionCharacter = Cast<AWildOmissionCharacter>(GetCharacter());
	if (WildOmissionCharacter == nullptr)
	{
		return PlayerSave;
	}

	PlayerSave.UniqueID = CurrentPlayerState->GetUniqueId().ToString();

	PlayerSave.WorldLocation = WildOmissionCharacter->GetActorLocation();
	PlayerSave.IsAlive = true;
	
	PlayerSave.Vitals.Health = WildOmissionCharacter->GetVitalsComponent()->GetHealth();
	PlayerSave.Vitals.Hunger = WildOmissionCharacter->GetVitalsComponent()->GetHunger();
	PlayerSave.Vitals.Thirst = WildOmissionCharacter->GetVitalsComponent()->GetThirst();

	PlayerSave.Inventory = WildOmissionCharacter->GetInventoryComponent()->Save();
	PlayerSave.SelectedItem = WildOmissionCharacter->GetInventoryManipulatorComponent()->GetSelectedItem();

	return PlayerSave;
}

void AWildOmissionPlayerController::LoadPlayerSave(const FWildOmissionPlayerSave& PlayerSave)
{
	AWildOmissionCharacter* WildOmissionCharacter = Cast<AWildOmissionCharacter>(GetCharacter());

	if (WildOmissionCharacter == nullptr)
	{
		return;
	}

	WildOmissionCharacter->SetActorLocation(PlayerSave.WorldLocation);
	
	WildOmissionCharacter->GetVitalsComponent()->SetHealth(PlayerSave.Vitals.Health);
	WildOmissionCharacter->GetVitalsComponent()->SetHunger(PlayerSave.Vitals.Hunger);
	WildOmissionCharacter->GetVitalsComponent()->SetThirst(PlayerSave.Vitals.Thirst);

	WildOmissionCharacter->GetInventoryComponent()->Load(PlayerSave.Inventory);

	if (PlayerSave.SelectedItem.Quantity > 0)
	{
		WildOmissionCharacter->GetInventoryManipulatorComponent()->SpawnWorldItem(PlayerSave.SelectedItem);
	}
}

void AWildOmissionPlayerController::Save()
{
	Server_AddToPendingSaves();

	if (HasAuthority() == false)
	{
		return;
	}
	
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	GameMode->SaveGame();
}

void AWildOmissionPlayerController::Spawn()
{
	// TODO find a location and give the default items
}

FString AWildOmissionPlayerController::GetUniqueID()
{
	FString ID = FString("");
	
	if (PlayerState)
	{
		ID = PlayerState->GetUniqueId().ToString();
	}

	return ID;
}

void AWildOmissionPlayerController::Server_AddToPendingSaves_Implementation()
{
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	GameMode->GetSaveHandler()->GetPlayerHandler()->AddToPending(this);
}

//*****************************
// Testing functions

void AWildOmissionPlayerController::LogLocalInventoryContents()
{
	if (!IsLocalController())
	{
		return;
	}
	AWildOmissionCharacter* WildOmissionCharacter = Cast<AWildOmissionCharacter>(GetPawn());
	if (WildOmissionCharacter == nullptr)
	{
		return;
	}

	// Get their inventory components
	for (const FInventoryItem& ItemData : GetPawn()->FindComponentByClass<UPlayerInventoryComponent>()->GetContents()->Contents)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Orange, FString::Printf(TEXT("Item: %s, Quantity: %i"), *ItemData.Name.ToString(), ItemData.Quantity));
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Green, FString::Printf(TEXT("Player: %s"), *WildOmissionCharacter->GetActorNameOrLabel()));
}