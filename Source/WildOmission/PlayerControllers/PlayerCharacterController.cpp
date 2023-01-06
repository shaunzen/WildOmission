// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerCharacterController.h"
#include "../Widgets/InventoryWidget.h"
#include "../ActorComponents/InventoryComponent.h"

void APlayerCharacterController::Server_DestroyActor_Implementation(AActor* ActorToDestroy)
{
	if (ActorToDestroy == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor to destroy was nullptr."));
		return;
	}
	ActorToDestroy->Destroy();
}

// TODO Add item server function here with a pointer to the inventory component to add to