// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerCharacterController.h"
#include "../Characters/PlayerCharacter.h"
#include "../ActorComponents/InventoryComponent.h"

void APlayerCharacterController::LogLocalInventoryContents()
{
	if (IsLocalController())
	{
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
}

void APlayerCharacterController::Server_DestroyActor_Implementation(AActor* ActorToDestroy)
{
	if (ActorToDestroy == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor to destroy was nullptr."));
		return;
	}
	ActorToDestroy->Destroy();
}