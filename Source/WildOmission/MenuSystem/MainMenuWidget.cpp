// Copyright Telephone Studios. All Rights Reserved.


#include "MainMenuWidget.h"
#include "SaveRowWidget.h"
#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/SaveGames/WildOmissionSaveGame.h"
#include "WildOmission/WildOmissionGameInstance.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsFocusable = true;
	
	ConstructorHelpers::FClassFinder<USaveRowWidget> SaveRowWidgetBPClass(TEXT("/Game/Blueprints/MenuSystem/WBP_SaveRow"));
	if (SaveRowWidgetBPClass.Class == nullptr)
	{
		return;
	}
	SaveRowWidgetClass = SaveRowWidgetBPClass.Class;

	ConstructorHelpers::FClassFinder<UServerRowWidget> ServerRowWidgetBPClass(TEXT("/Game/Blueprints/MenuSystem/WBP_ServerRow"));
	if (ServerRowWidgetBPClass.Class == nullptr)
	{
		return;
	}
	ServerRowWidgetClass = ServerRowWidgetBPClass.Class;
}

bool UMainMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (Success == false
		|| SingleplayerButton == nullptr
		|| MultiplayerButton == nullptr
		|| ExitButton == nullptr
		|| SingleplayerSelectSaveButton == nullptr
		|| SingleplayerNewSaveButton == nullptr
		|| SingleplayerBackButton == nullptr
		|| MultiplayerJoinButton == nullptr
		|| MultiplayerHostButton == nullptr
		|| MultiplayerBackButton == nullptr
		|| RefreshServerListButton == nullptr
		|| NewSaveCreateButton == nullptr
		|| NewSaveBackButton == nullptr
		|| HostMenuHostButton == nullptr
		|| HostMenuBackButton == nullptr)
	{
		return false;
	}

	// Bind button delegates
	/*Main Menu*/
	SingleplayerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenSingleplayerMenu);
	MultiplayerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMultiplayerMenu);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitGame);
	
	/*Singleplayer*/
	SingleplayerSelectSaveButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LoadSave);
	SingleplayerNewSaveButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenNewSaveMenu);
	SingleplayerBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);

	/*Multiplayer*/
	MultiplayerJoinButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinServer);
	MultiplayerHostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenHostMenu);
	MultiplayerBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
	RefreshServerListButton->OnClicked.AddDynamic(this, &UMainMenuWidget::RefreshServerList);
	/*New Save*/
	NewSaveCreateButton->OnClicked.AddDynamic(this, &UMainMenuWidget::CreateSave);
	NewSaveBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenSingleplayerMenu);

	/*Host Server*/
	HostMenuHostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostServer);
	HostMenuBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMultiplayerMenu);

	return true;
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

void UMainMenuWidget::SetSaveList(TArray<FString> SaveNames)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	SaveList->ClearChildren();
	HostSaveList->ClearChildren();

	uint32 i = 0;
	for (const FString& SaveName : SaveNames)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Save: %s"), *SaveName);
		
		UWildOmissionSaveGame* SaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
		USaveRowWidget* Row = CreateWidget<USaveRowWidget>(World, SaveRowWidgetClass);
		USaveRowWidget* HostRow = CreateWidget<USaveRowWidget>(World, SaveRowWidgetClass);
		if (Row == nullptr || HostRow == nullptr || SaveGame == nullptr)
		{
			return;
		}
		
		FString DaysPlayedString = FString::Printf(TEXT("%i Days"), SaveGame->DaysPlayed);
		FString CreationString = FString::Printf(TEXT("Created: %i/%i/%i"), SaveGame->CreationInformation.Month, SaveGame->CreationInformation.Day, SaveGame->CreationInformation.Year);

		Row->SaveName->SetText(FText::FromString(SaveName));
		Row->DaysPlayed->SetText(FText::FromString(DaysPlayedString));
		Row->DateCreated->SetText(FText::FromString(CreationString));
		Row->Setup(this, i);

		HostRow->SaveName->SetText(FText::FromString(SaveName));
		HostRow->DaysPlayed->SetText(FText::FromString(DaysPlayedString));
		HostRow->DateCreated->SetText(FText::FromString(CreationString));
		HostRow->Setup(this, i);
		++i;

		SaveList->AddChild(Row);
		HostSaveList->AddChild(HostRow);
	}
}

void UMainMenuWidget::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRowWidget* Row = CreateWidget<UServerRowWidget>(World, ServerRowWidgetClass);
		if (Row == nullptr)
		{
			return;
		}

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		FString HostString = FString::Printf(TEXT("Host: %s"), *ServerData.HostUsername);
		Row->HostUser->SetText(FText::FromString(HostString));
		FString FractionString = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionString));
		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}

	FString RefreshString = FString("Refresh");
	RefreshServerListButtonText->SetText(FText::FromString(RefreshString));
}

void UMainMenuWidget::SelectSaveIndex(uint32 Index)
{
	SelectedSaveIndex = Index;
	UpdateSaveListChildren();
}

void UMainMenuWidget::SelectServerIndex(uint32 Index)
{
	SelectedServerIndex = Index;
	UpdateServerListChildren();
}

void UMainMenuWidget::UpdateSaveListChildren()
{
	for (int32 i = 0; i < SaveList->GetChildrenCount(); ++i)
	{
		USaveRowWidget* Row = Cast<USaveRowWidget>(SaveList->GetChildAt(i));
		USaveRowWidget* HostRow = Cast<USaveRowWidget>(HostSaveList->GetChildAt(i));
		
		if (Row == nullptr || HostRow == nullptr)
		{
			return;
		}

		bool RowSelected = (SelectedSaveIndex.IsSet() && SelectedSaveIndex.GetValue() == i);

		Row->Selected = RowSelected;
		HostRow->Selected = RowSelected;
	}
}

void UMainMenuWidget::UpdateServerListChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRowWidget* Row = Cast<UServerRowWidget>(ServerList->GetChildAt(i));
		if (Row == nullptr)
		{
			return;
		}
		Row->Selected = (SelectedServerIndex.IsSet() && SelectedServerIndex.GetValue() == i);
	}
}

//****************************
// Menu Functions
//****************************
void UMainMenuWidget::OpenMainMenu()
{
	if (MenuSwitcher == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenuWidget::OpenSingleplayerMenu()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());

	if (MenuSwitcher == nullptr || GameInstance == nullptr)
	{
		return;
	}
	
	SetSaveList(GameInstance->GetAllSaveGameSlotNames());
	MenuSwitcher->SetActiveWidget(SingleplayerMenu);
}

void UMainMenuWidget::OpenMultiplayerMenu()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (MenuSwitcher == nullptr || GameInstance == nullptr)
	{
		return;
	}

	FString WaitingString = FString("...");
	RefreshServerListButtonText->SetText(FText::FromString(WaitingString));

	MenuSwitcher->SetActiveWidget(MultiplayerMenu);
	GameInstance->RefreshServerList();
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

void UMainMenuWidget::OpenNewSaveMenu()
{
	if (MenuSwitcher == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(NewSaveMenu);
}

void UMainMenuWidget::OpenHostMenu()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (MenuSwitcher == nullptr || GameInstance == nullptr)
	{
		return;
	}

	SetSaveList(GameInstance->GetAllSaveGameSlotNames());
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenuWidget::LoadSave()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	
	if (GameInstance == nullptr || SelectedSaveIndex.IsSet() == false)
	{
		return;
	}

	FString SaveName;
	SaveName = GameInstance->GetAllSaveGameSlotNames()[SelectedSaveIndex.GetValue()];

	GameInstance->StartSingleplayer(SaveName);
}

void UMainMenuWidget::CreateSave()
{
	// Get the name of the save
	FString NewSaveName;
	NewSaveName = SaveNameInputBox->GetText().ToString();

	// Create a new save with that name
	UWildOmissionSaveGame* NewSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
	UGameplayStatics::SaveGameToSlot(NewSaveGame, NewSaveName, 0);

	// Start singleplayer with that new save
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	GameInstance->StartSingleplayer(NewSaveName);
}

void UMainMenuWidget::JoinServer()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());

	if (GameInstance == nullptr || SelectedServerIndex.IsSet() == false)
	{
		return;
	}

	GameInstance->Join(SelectedServerIndex.GetValue());
}

void UMainMenuWidget::HostServer()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (GameInstance == nullptr || SelectedSaveIndex.IsSet() == false)
	{
		return;
	}

	FString ServerName = ServerNameInputBox->GetText().ToString();
	FString SaveName = GameInstance->GetAllSaveGameSlotNames()[SelectedSaveIndex.GetValue()];

	GameInstance->Host(ServerName, SaveName);
}

void UMainMenuWidget::RefreshServerList()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	
	if (GameInstance == nullptr)
	{
		return;
	}

	FString WaitingString = FString("...");
	RefreshServerListButtonText->SetText(FText::FromString(WaitingString));
	
	GameInstance->RefreshServerList();
}