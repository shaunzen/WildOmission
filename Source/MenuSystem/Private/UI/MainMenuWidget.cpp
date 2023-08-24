// Copyright Telephone Studios. All Rights Reserved.


#include "UI/MainMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "WorldSelectionWidget.h"
#include "WorldMenuWidget.h"
#include "Interfaces/GameSaveLoadController.h"
#include "UI/OptionsWidget.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);
	MenuSwitcher = nullptr;
	PlayButton = nullptr;
	OptionsButton = nullptr;
	ExitButton = nullptr;
	MainMenu = nullptr;
	WorldSelectionMenu = nullptr;
	WorldMenu = nullptr;
	OptionsMenu = nullptr;
	CreditsMenu = nullptr;
	MenuInterface = nullptr;
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);
	OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenOptionsMenu);
	CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenCreditsMenu);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitGame);

	WorldSelectionMenu->Setup(this);
	WorldMenu->Setup(this);
	OptionsMenu->OnBackButtonPressed.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
}

void UMainMenuWidget::Setup(IMenuInterface* InMenuInterface)
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

	MenuInterface = InMenuInterface;
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

IMenuInterface* UMainMenuWidget::GetMenuInterface() const
{
	return MenuInterface;
}

void UMainMenuWidget::OpenMainMenu()
{
	if (MenuSwitcher == nullptr || MainMenu == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to switch to main menu"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Switching to main menu"));
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenuWidget::OpenWorldSelectionMenu()
{
	if (MenuSwitcher == nullptr || WorldSelectionMenu == nullptr || MenuInterface == nullptr)
	{
		return;
	}

	WorldSelectionMenu->SetWorldList(MenuInterface->GetAllWorldNames());
	MenuSwitcher->SetActiveWidget(WorldSelectionMenu);
}

void UMainMenuWidget::CreateDemoWorld()
{
	IGameSaveLoadController* GameSaveLoadController = Cast<IGameSaveLoadController>(GetGameInstance());
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	const FString DemoWorldName = TEXT("Demo World");
	GameSaveLoadController->CreateWorld(DemoWorldName);
	OpenWorldMenuForWorld(DemoWorldName);
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

void UMainMenuWidget::OpenOptionsMenu()
{
	if (MenuSwitcher == nullptr || OptionsMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(OptionsMenu);
	OptionsMenu->Refresh();
}

void UMainMenuWidget::OpenCreditsMenu()
{
	if (MenuSwitcher == nullptr || CreditsMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(CreditsMenu);
}

void UMainMenuWidget::ExitGame()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ConsoleCommand(TEXT("quit"));
}
