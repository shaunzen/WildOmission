// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionPlayerController.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/UI/Player/Inventory/InventoryWidget.h"
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

	PlayerSave.Inventory = WildOmissionCharacter->GetInventoryComponent()->Save();

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
	WildOmissionCharacter->GetInventoryComponent()->Load(PlayerSave.Inventory);
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
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString("Player hasnt played on this save or was dead last time they loaded, spawning with default settings"));
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
	for (const FInventoryItem& ItemData : WildOmissionCharacter->GetInventoryComponent()->GetContents()->Contents)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Orange, FString::Printf(TEXT("Item: %s, Quantity: %i"), *ItemData.Name.ToString(), ItemData.Quantity));
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Green, FString::Printf(TEXT("Player: %s"), *WildOmissionCharacter->GetActorNameOrLabel()));
}

void AWildOmissionPlayerController::LogLocalInventorySlots()
{
	if (IsLocalController() == false)
	{
		return;
	}
	AWildOmissionCharacter* WildOmissionCharacter = Cast<AWildOmissionCharacter>(GetPawn());
	if (WildOmissionCharacter == nullptr)
	{
		return;
	}

	for (const FInventorySlot& Slot : WildOmissionCharacter->GetInventoryComponent()->GetSlots())
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Orange, FString::Printf(TEXT("Index: %i, Item: %s, Quantity: %i"), Slot.Index, *Slot.Item.Name.ToString(), Slot.Item.Quantity));
	}
}
void AWildOmissionPlayerController::Server_DestroyActor_Implementation(AActor* ActorToDestroy)
{
	if (ActorToDestroy == nullptr)
	{
		return;
	}

	ActorToDestroy->Destroy();
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