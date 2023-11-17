// Copyright Telephone Studios. All Rights Reserved.


#include "UI/MainMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "WorldSelectionWidget.h"
#include "WorldCreationWidget.h"
#include "WorldMenuWidget.h"
#include "RenameWorldWidget.h"
#include "DeleteWorldWidget.h"
#include "ServerBrowserWidget.h"
#include "UI/OptionsWidget.h"
#include "ErrorMessagePrompt.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Log.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);

	MenuSwitcher = nullptr;
	PlayButton = nullptr;
	OptionsButton = nullptr;
	FeedbackButton = nullptr;
	ExitButton = nullptr;
	MainMenu = nullptr;
	WorldSelectionMenu = nullptr;
	WorldCreationMenu = nullptr;
	WorldMenu = nullptr;
	RenameWorldMenu = nullptr;
	DeleteWorldMenu = nullptr;
	ServerBrowserMenu = nullptr;
	OptionsMenu = nullptr;
	ErrorMessagePrompt = nullptr;
	MenuInterface = nullptr;
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);
	OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenOptionsMenu);
	FeedbackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenFeedbackPage);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitGame);

	WorldSelectionMenu->OnSelectButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldMenu);
	WorldSelectionMenu->OnCreateNewWorldButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldCreationMenu);
	WorldSelectionMenu->OnMultiplayerButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenServerBrowserMenu);
	WorldSelectionMenu->OnCancelButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);

	WorldCreationMenu->OnOpenWorldMenuRequested.AddDynamic(this, &UMainMenuWidget::OpenWorldMenuForWorld);
	WorldCreationMenu->OnCancelButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);

	WorldMenu->OnPlayButtonClicked.AddDynamic(this, &UMainMenuWidget::HostGame);
	WorldMenu->OnRenameButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenRenameWorldMenu);
	WorldMenu->OnDeleteButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenDeleteWorldMenu);
	WorldMenu->OnCancelButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);

	RenameWorldMenu->OnRenameButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldMenuForWorld);
	RenameWorldMenu->OnCancelButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldMenu);

	DeleteWorldMenu->OnDeleteButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);
	DeleteWorldMenu->OnCancelButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldMenu);

	ServerBrowserMenu->OnJoinButtonClicked.AddDynamic(this, &UMainMenuWidget::JoinServer);
	ServerBrowserMenu->OnRefreshButtonClicked.AddDynamic(this, &UMainMenuWidget::RefreshServerList);
	ServerBrowserMenu->OnCancelButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);

	OptionsMenu->OnBackButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);

	ErrorMessagePrompt->OnCloseButtonClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
}

void UMainMenuWidget::SetServerList(TArray<FServerData> InServerData)
{
	ServerBrowserMenu->SetServerList(InServerData);
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

void UMainMenuWidget::JoinServer(const uint32& ServerIndex)
{
	if (MenuInterface == nullptr)
	{
		return;
	}

	MenuInterface->JoinServer(ServerIndex);
}

void UMainMenuWidget::RefreshServerList()
{
	if (MenuInterface == nullptr)
	{
		return;
	}

	MenuInterface->RefreshServerList();
}

void UMainMenuWidget::OpenMainMenu()
{
	if (MenuSwitcher == nullptr || MainMenu == nullptr)
	{
		UE_LOG(LogMenuSystem, Warning, TEXT("Failed to switch to main menu"));
		return;
	}
	UE_LOG(LogMenuSystem, Verbose, TEXT("Switching to main menu"));
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

void UMainMenuWidget::HostGame(const FString& WorldName, const FString& ServerName, const bool IsMultiplayer, const bool IsFriendsOnly)
{
	if (MenuInterface == nullptr || WorldName.IsEmpty())
	{
		return;
	}

	IsMultiplayer ? MenuInterface->HostServer(ServerName, WorldName, IsFriendsOnly) : MenuInterface->StartSingleplayer(WorldName);
}

void UMainMenuWidget::OpenRenameWorldMenu()
{
	if (MenuSwitcher == nullptr || RenameWorldMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(RenameWorldMenu);
	RenameWorldMenu->Open(WorldSelectionMenu->SelectedWorldName.GetValue());
}

void UMainMenuWidget::OpenDeleteWorldMenu()
{
	if (MenuSwitcher == nullptr || DeleteWorldMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(DeleteWorldMenu);
	DeleteWorldMenu->Open(WorldSelectionMenu->SelectedWorldName.GetValue());
}

void UMainMenuWidget::OpenServerBrowserMenu()
{
	if (MenuSwitcher == nullptr || ServerBrowserMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(ServerBrowserMenu);
	ServerBrowserMenu->OnOpen();
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

void UMainMenuWidget::OpenFeedbackPage()
{
	UKismetSystemLibrary::LaunchURL(TEXT("https://forms.gle/2GP8ZSTU5ARa5Pmu8"));
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

void UMainMenuWidget::OpenErrorPrompt(const FString& Title, const FString& Error)
{
	if (ErrorMessagePrompt == nullptr || MenuSwitcher == nullptr)
	{
		UE_LOG(LogMenuSystem, Warning, TEXT("MainMenuWidget::OpenErrorPrompt, ErrorMessagePrompt or MenuSwitcher was nullptr."));
		return;
	}

	MenuSwitcher->SetActiveWidget(ErrorMessagePrompt);
	ErrorMessagePrompt->SetMessage(Title, Error);
}
