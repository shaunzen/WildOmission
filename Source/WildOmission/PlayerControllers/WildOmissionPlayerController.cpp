// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionPlayerController.h"
#include "WildOmission/Characters/PlayerCharacter.h"
#include "WildOmission/ActorComponents/InventoryComponent.h"
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
	PlayerSave.ID = CurrentPlayerState->GetPlayerId();
	PlayerSave.WorldLocation = GetPawn()->GetActorLocation();

	return PlayerSave;
}

void AWildOmissionPlayerController::LoadPlayerSave(const FWildOmissionPlayerSave& PlayerSave)
{
	GetPawn()->SetActorLocation(PlayerSave.WorldLocation);
}

void AWildOmissionPlayerController::LogLocalInventoryContents()
{
	if (!IsLocalController())
	{
		return;
	}
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter == nullptr)
	{
		return;
	}

	// Get their inventory components
	for (TPair<FName, int32>& Item : *PlayerCharacter->GetInventoryComponent()->GetContents())
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Orange, FString::Printf(TEXT("Item: %s, Quantity: %i"), *Item.Key.ToString(), Item.Value));
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Green, FString::Printf(TEXT("Player: %s"), *PlayerCharacter->GetActorNameOrLabel()));
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