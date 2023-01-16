// Copyright Telephone Studios. All Rights Reserved.


#include "GameplayMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WildOmission/WildOmissionGameInstance.h"
#include "WildOmission/GameModes/WildOmissionGameMode.h"

UGameplayMenuWidget::UGameplayMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsFocusable = true;
	bOpen = false;
}

bool UGameplayMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success || ResumeButton == nullptr || SaveButton == nullptr || QuitToMenuButton == nullptr)
	{
		return false;
	}

	// Bind button delegates
	ResumeButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::Teardown);
	SaveButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::SaveGame);
	QuitToMenuButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::QuitToMenu);

	return true;
}

void UGameplayMenuWidget::Show()
{
	bOpen = true;
	AddToViewport();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

bool UGameplayMenuWidget::IsOpen() const
{
	return bOpen;
}

void UGameplayMenuWidget::Teardown()
{
	bOpen = false;
	RemoveFromParent();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UGameplayMenuWidget::SaveGame()
{
	// Get the game mode
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save game: not the server"));
		return;
	}

	GameMode->SaveGame();
}

void UGameplayMenuWidget::QuitToMenu()
{
	// TODO call quit to menu
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		return;
	}
	GameInstance->ReturnToMainMenu();
}