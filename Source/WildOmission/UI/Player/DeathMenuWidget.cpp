// Copyright Telephone Studios. All Rights Reserved.


#include "DeathMenuWidget.h"
#include "Components/Button.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"

UDeathMenuWidget::UDeathMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsFocusable = true;
}

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
	AWildOmissionPlayerController* PlayerController = Cast<AWildOmissionPlayerController>(GetOwningPlayer());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to respawn player couldnt get player controller"));
		return;
	}

	PlayerController->UnPossess();
	PlayerController->ServerRestartPlayer();

	Teardown();
}

void UDeathMenuWidget::QuitButtonClicked()
{
	// TODO open main menu
	Teardown();
}

void UDeathMenuWidget::Teardown()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to teardown death menu, couldn't get owning player controller"));
		return;
	}

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;

	this->RemoveFromParent();
}