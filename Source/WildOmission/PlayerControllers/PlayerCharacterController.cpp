// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerCharacterController.h"

void APlayerCharacterController::Server_DestroyActor_Implementation(AActor* ActorToDestroy)
{
	if (ActorToDestroy == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor to destroy was nullptr."));
		return;
	}
	ActorToDestroy->Destroy();
}