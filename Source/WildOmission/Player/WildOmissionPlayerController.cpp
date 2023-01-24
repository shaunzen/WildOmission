// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionPlayerController.h"
#include "WildOmissionCharacter.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "GameFramework/PlayerState.h"

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
	PlayerSave.UniqueID = CurrentPlayerState->GetUniqueId().ToString();

	PlayerSave.WorldLocation = GetPawn()->GetActorLocation();
	PlayerSave.IsAlive = true;

	return PlayerSave;
}

void AWildOmissionPlayerController::LoadPlayerSave(const FWildOmissionPlayerSave& PlayerSave)
{
	GetPawn()->SetActorLocation(PlayerSave.WorldLocation);
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
	for (TPair<FName, int32>& Item : *WildOmissionCharacter->GetInventoryComponent()->GetContents())
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Orange, FString::Printf(TEXT("Item: %s, Quantity: %i"), *Item.Key.ToString(), Item.Value));
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Green, FString::Printf(TEXT("Player: %s"), *WildOmissionCharacter->GetActorNameOrLabel()));
}

void AWildOmissionPlayerController::Server_DestroyActor_Implementation(AActor* ActorToDestroy)
{
	if (ActorToDestroy == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor to destroy was nullptr."));
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Black, FString::Printf(TEXT("Destroying Actor Named: %s"), *ActorToDestroy->GetActorNameOrLabel()));

	ActorToDestroy->Destroy();
}