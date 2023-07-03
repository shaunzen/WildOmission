// Copyright Telephone Studios. All Rights Reserved.


#include "DeathMenuWidget.h"
#include "Components/Button.h"
#include "WildOmissionCore/PlayerControllers/WildOmissionPlayerController.h"
#include "WildOmissionCore/WildOmissionGameInstance.h"

UDeathMenuWidget::UDeathMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);
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

void UDeathMenuWidget::Show()
{
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

void UDeathMenuWidget::RespawnButtonClicked()
{
	AWildOmissionPlayerController* PlayerController = Cast<AWildOmissionPlayerController>(GetOwningPlayer());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to respawn player couldnt get player controller"));
		return;
	}

	PlayerController->UnPossess();
	PlayerController->Server_Spawn();

	Teardown();
}

void UDeathMenuWidget::QuitButtonClicked()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());

	if (!GameInstance)
	{
		return;
	}

	GameInstance->ReturnToMainMenu();

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