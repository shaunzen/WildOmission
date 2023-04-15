// Copyright Telephone Studios. All Rights Reserved.


#include "MainMenuWidget.h"
#include "WorldRowWidget.h"
#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "CreateWorldButtonWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Core/SaveSystem/WildOmissionSaveGame.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsFocusable = true;
	
	ConstructorHelpers::FClassFinder<UWorldRowWidget> WorldRowWidgetBPClass(TEXT("/Game/WildOmission/UI/Menu/WBP_WorldRow"));
	if (WorldRowWidgetBPClass.Class == nullptr)
	{
		return;
	}
	WorldRowWidgetClass = WorldRowWidgetBPClass.Class;

	ConstructorHelpers::FClassFinder<UCreateWorldButtonWidget> CreateNewWorldBPClass(TEXT("/Game/WildOmission/UI/Menu/WBP_CreateWorldButton"));
	if (CreateNewWorldBPClass.Class == nullptr)
	{
		return;
	}
	CreateNewWorldButtonClass = CreateNewWorldBPClass.Class;

	ConstructorHelpers::FClassFinder<UServerRowWidget> ServerRowWidgetBPClass(TEXT("/Game/WildOmission/UI/Menu/WBP_ServerRow"));
	if (ServerRowWidgetBPClass.Class == nullptr)
	{
		return;
	}
	ServerRowWidgetClass = ServerRowWidgetBPClass.Class;
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button delegates
	
	/*Main Menu*/
	PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);
	// TODO Settings Menu
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitGame);

	/*World Selection Menu*/
	WorldSelectionSelectButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldMenu);
	WorldSelectionBrowseServersButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenServerBrowserMenu);
	WorldSelectionBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
	
	/*World Creation Menu*/
	WorldCreationCreateWorldButton->OnClicked.AddDynamic(this, &UMainMenuWidget::CreateWorld);
	WorldCreationBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);
	WorldNameInputBox->OnTextChanged.AddDynamic(this, &UMainMenuWidget::WorldNameOnTextChanged);

	/*World Menu*/
	PlayWorldButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlaySelectedWorld);
	WorldBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldSelectionMenu);
	MultiplayerCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainMenuWidget::MultiplayerCheckboxChanged);
	ServerNameInputBox->OnTextChanged.AddDynamic(this, &UMainMenuWidget::ServerNameOnTextChanged);
	
	/*Server Browser*/
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
	
	WorldListBox->ClearChildren();
	
	uint32 i = 0;
	for (const FString& WorldName : SaveNames)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Save: %s"), *WorldName);
		
		UWildOmissionSaveGame* SaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(WorldName, 0));
		UWorldRowWidget* Row = CreateWidget<UWorldRowWidget>(World, WorldRowWidgetClass);
		if (Row == nullptr || SaveGame == nullptr)
		{
			return;
		}
		
		FString DaysPlayedString = FString::Printf(TEXT("%i Days"), SaveGame->DaysPlayed);
		FString CreationString = FString::Printf(TEXT("Created: %i/%i/%i"), SaveGame->CreationInformation.Month, SaveGame->CreationInformation.Day, SaveGame->CreationInformation.Year);

		Row->WorldNameTextBlock->SetText(FText::FromString(WorldName));
		Row->DaysPlayed->SetText(FText::FromString(DaysPlayedString));
		Row->DateCreated->SetText(FText::FromString(CreationString));
		Row->Setup(this, WorldName);

		++i;

		WorldListBox->AddChild(Row);
	}

	UCreateWorldButtonWidget* CreateWorldButton = CreateWidget<UCreateWorldButtonWidget>(World, CreateNewWorldButtonClass);
	if (CreateWorldButton == nullptr)
	{
		return;
	}
	
	CreateWorldButton->GetButton()->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenWorldCreationMenu);

	WorldListBox->AddChild(CreateWorldButton);
}

void UMainMenuWidget::SetServerList(TArray<FServerData> ServerNames)
{
	//UWorld* World = GetWorld();

	//if (World == nullptr)
	//{
	//	return;
	//}

	//ServerList->ClearChildren();

	//uint32 i = 0;
	//for (const FServerData& ServerData : ServerNames)
	//{
	//	UServerRowWidget* Row = CreateWidget<UServerRowWidget>(World, ServerRowWidgetClass);
	//	if (Row == nullptr)
	//	{
	//		return;
	//	}

	//	Row->ServerName->SetText(FText::FromString(ServerData.Name));
	//	FString HostString = FString::Printf(TEXT("Host: %s"), *ServerData.HostUsername);
	//	Row->HostUser->SetText(FText::FromString(HostString));
	//	FString FractionString = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
	//	Row->ConnectionFraction->SetText(FText::FromString(FractionString));
	//	Row->Setup(this, i);
	//	++i;

	//	ServerList->AddChild(Row);
	//}

	//FString RefreshString = FString("Refresh");
	//RefreshServerListButtonText->SetText(FText::FromString(RefreshString));
}

void UMainMenuWidget::SetSelectedWorld(const FString& WorldName)
{
	SelectedWorldName = WorldName;
	UpdateSaveListChildren();
}

void UMainMenuWidget::SelectServerIndex(uint32 Index)
{
	SelectedServerIndex = Index;
	UpdateServerListChildren();
}

void UMainMenuWidget::UpdateSaveListChildren()
{
	for (int32 i = 0; i < WorldListBox->GetChildrenCount(); ++i)
	{
		UWorldRowWidget* Row = Cast<UWorldRowWidget>(WorldListBox->GetChildAt(i));
		
		if (Row == nullptr)
		{
			return;
		}

		bool RowSelected = (SelectedWorldName.IsSet() && SelectedWorldName.GetValue() == Row->GetWorldName());

		Row->Selected = RowSelected;
	}
}

void UMainMenuWidget::UpdateServerListChildren()
{
	/*for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRowWidget* Row = Cast<UServerRowWidget>(ServerList->GetChildAt(i));
		if (Row == nullptr)
		{
			return;
		}
		Row->Selected = (SelectedServerIndex.IsSet() && SelectedServerIndex.GetValue() == i);
	}*/
}

//****************************
// Menu Functions
//****************************
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

	SetSaveList(GameInstance->GetAllWorldNames());
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
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (GameInstance == nullptr || MenuSwitcher == nullptr || WorldMenu == nullptr || SelectedWorldName.IsSet() == false)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(WorldMenu);
	WorldMenuTitle->SetText(FText::FromString(SelectedWorldName.GetValue()));
}

void UMainMenuWidget::OpenServerBrowserMenu()
{
	if (MenuSwitcher == nullptr || ServerBrowserMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(ServerBrowserMenu);
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

void UMainMenuWidget::PlaySelectedWorld()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	
	if (GameInstance == nullptr || SelectedWorldName.IsSet() == false)
	{
		return;
	}

	if (MultiplayerCheckBox->IsChecked())
	{
		FString ServerName = ServerNameInputBox->GetText().ToString();
		if (ServerName == FString(""))
		{
			return;
		}

		GameInstance->Host(ServerName, SelectedWorldName.GetValue());
	}
	else
	{
		GameInstance->StartSingleplayer(SelectedWorldName.GetValue());
	}
}

void UMainMenuWidget::CreateWorld()
{
	// Get the name of the save
	FString NewWorldName;
	NewWorldName = WorldNameInputBox->GetText().ToString();

	if (NewWorldName == FString(""))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot create a world without a name."));
		return;
	}
	
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	// Create a new world with that name
	GameInstance->CreateWorld(NewWorldName);
	
	// Set as selected world
	SelectedWorldName = NewWorldName;

	// Back to Selection Menu
	OpenWorldMenu();
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


void UMainMenuWidget::RefreshServerList()
{
	//UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	//
	//if (GameInstance == nullptr)
	//{
	//	return;
	//}

	//FString WaitingString = FString("...");
	//RefreshServerListButtonText->SetText(FText::FromString(WaitingString));
	//
	//GameInstance->RefreshServerList();
}

void UMainMenuWidget::WorldNameOnTextChanged(const FText& Text)
{
	FString TextString = Text.ToString();

	if (TextString.Len() > 16)
	{
		TextString = TextString.LeftChop(1);
	}

	WorldNameInputBox->SetText(FText::FromString(TextString));
}

void UMainMenuWidget::ServerNameOnTextChanged(const FText& Text)
{
	FString TextString = Text.ToString();

	if (TextString.Len() > 16)
	{
		TextString = TextString.LeftChop(1);
	}

	ServerNameInputBox->SetText(FText::FromString(TextString));
}

void UMainMenuWidget::MultiplayerCheckboxChanged(bool bIsChecked)
{
	HostSettingsMenu->SetIsEnabled(bIsChecked);
}