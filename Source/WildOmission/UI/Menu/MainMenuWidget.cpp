// Copyright Telephone Studios. All Rights Reserved.


#include "MainMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "WorldSelectionWidget.h"
#include "ServerBrowserWidget.h"
#include "WorldMenuWidget.h"
#include "WorldCreationWidget.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);
	// TODO Settings Menu
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitGame);

	WorldSelectionMenu->Setup(this);
	ServerBrowserMenu->Setup(this);
	WorldMenu->Setup(this);
	WorldCreationMenu->Setup(this);
}

void UMainMenuWidget::SetServerList(TArray<FServerData> InServerData)
{
	ServerBrowserMenu->SetServerList(InServerData);
}

void UMainMenuWidget::Setup()
{
	this->AddToViewport();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(this->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
	}
}

void UMainMenuWidget::Teardown()
{
	this->RemoveFromParent();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
	}
}

void UMainMenuWidget::OpenMainMenu()
{
	if (MenuSwitcher == nullptr || MainMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenuWidget::OpenWorldSelectionMenu()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (MenuSwitcher == nullptr || WorldSelectionMenu == nullptr || GameInstance == nullptr)
	{
		return;
	}

	WorldSelectionMenu->SetWorldList(GameInstance->GetAllWorldNames());
	MenuSwitcher->SetActiveWidget(WorldSelectionMenu);
}

void UMainMenuWidget::OpenWorldCreationMenu()
{
	if (MenuSwitcher == nullptr || WorldCreationMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(WorldCreationMenu);
}

void UMainMenuWidget::OpenWorldMenu()
{
	if (MenuSwitcher == nullptr || WorldMenu == nullptr || WorldSelectionMenu->SelectedWorldName.IsSet() == false)
	{
		return;
	}

	OpenWorldMenuForWorld(WorldSelectionMenu->SelectedWorldName.GetValue());
}

void UMainMenuWidget::OpenWorldMenuForWorld(const FString& WorldName)
{
	MenuSwitcher->SetActiveWidget(WorldMenu);
	WorldMenu->Open(WorldName);
}

void UMainMenuWidget::OpenServerBrowserMenu()
{
	if (MenuSwitcher == nullptr || ServerBrowserMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(ServerBrowserMenu);
	ServerBrowserMenu->Open();
}

void UMainMenuWidget::ExitGame()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ConsoleCommand(FString("quit"));
}
