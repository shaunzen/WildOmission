// Copyright Telephone Studios. All Rights Reserved.


#include "MainMenuWidget.h"
#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "WildOmission/WildOmissionGameInstance.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsFocusable = true;

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
		|| SingleplayerBackButton == nullptr)
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

void UMainMenuWidget::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	//ServerList->ClearChildren();

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

		//ServerList->AddChild(Row);
	}
}

void UMainMenuWidget::SelectServerIndex(uint32 Index)
{
	SelectedServerIndex = Index;
	UpdateServerListChildren();
}

void UMainMenuWidget::UpdateServerListChildren()
{
	/*
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRowWidget* Row = Cast<UServerRowWidget>(ServerList->GetChildAt(i));
		if (Row == nullptr)
		{
			return;
		}
		Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
	}
	*/
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
	if (MenuSwitcher == nullptr)
	{
		return;
	}
	
	MenuSwitcher->SetActiveWidget(SingleplayerMenu);
}

void UMainMenuWidget::OpenMultiplayerMenu()
{
	if (MenuSwitcher == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(MultiplayerMenu);
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

void UMainMenuWidget::LoadSave()
{
	// TODO Load save
}