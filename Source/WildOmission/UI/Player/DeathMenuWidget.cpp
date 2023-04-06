// Copyright Telephone Studios. All Rights Reserved.


#include "DeathMenuWidget.h"
#include "Components/Button.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"

void UDeathMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RespawnButton == nullptr || QuitButton == nullptr)
	{
		return;
	}

	RespawnButton->OnClicked.AddDynamic(this, &UDeathMenuWidget::RespawnButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UDeathMenuWidget::QuitButtonClicked);
}

void UDeathMenuWidget::RespawnButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Respawning"));
	AWildOmissionPlayerController* PlayerController = Cast<AWildOmissionPlayerController>(GetOwningPlayer());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to respawn player couldnt get player controller"));
		return;
	}

	// RPC to server on player controller (Server_RequestRespawn)
	PlayerController->Server_RequestRespawn();
	// Player controller then tells the game mode to handle the spawning for us
}

void UDeathMenuWidget::QuitButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString("Quiting to menu"));
}