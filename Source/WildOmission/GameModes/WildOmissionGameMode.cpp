// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameMode.h"
#include "../Characters/PlayerCharacter.h"
#include "../ActorComponents/InventoryComponent.h"

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
		
	}
}