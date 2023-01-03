// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(Server, Reliable)
	void Server_DestroyActor(AActor* ActorToDestroy);
};
