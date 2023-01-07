// Copyright Telephone Studios. All Rights Reserved.


#include "MainMenuWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRowWidget.h"

void UMainMenuWidget::Setup()
{
	this->AddToViewport();
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
	OpenMainMenu();
	UE_LOG(LogTemp, Warning, TEXT("Finished setting up menu widget."));
}

void UMainMenuWidget::TearDown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
	UE_LOG(LogTemp, Warning, TEXT("Tearing down Menu widget."));
}

void UMainMenuWidget::SetMainMenuInterface(IMainMenuInterface* InMainMenuInterface)
{
	MainMenuInterface = InMainMenuInterface;
}

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowWidgetBPClass(TEXT("/Game/Blueprints/Widgets/WBP_ServerRow"));
	if (ServerRowWidgetBPClass.Class == nullptr)
	{
		return;
	}

	ServerRowWidgetClass = ServerRowWidgetBPClass.Class;
}

bool UMainMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (Success == false)
	{
		return false;
	}
	if (HostButton == nullptr
		|| CancelHostMenuButton == nullptr
		|| ConfirmHostMenuButton == nullptr
		|| JoinButton == nullptr
		|| QuitButton == nullptr
		|| CancelJoinMenuButton == nullptr
		|| ConfirmJoinMenuButton == nullptr)
	{
		return false;
	}
	HostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenHostMenu);
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
	ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenJoinMenu);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitPressed);
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinServer);
	
	UE_LOG(LogTemp, Warning, TEXT("Main Menu Widget Init"));
	return true;
}

void UMainMenuWidget::OpenHostMenu()
{
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenuWidget::HostServer()
{
	if (MainMenuInterface == nullptr)
	{
		return;
	}

	FString ServerName = ServerHostName->Text.ToString();
	MainMenuInterface->Host(ServerName);
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
		Row->HostUser->SetText(FText::FromString(ServerData.HostUsername));
		FString FractionString = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionString));
		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}
}

void UMainMenuWidget::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenuWidget::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRowWidget* Row = Cast<UServerRowWidget>(ServerList->GetChildAt(i));
		if (Row == nullptr)
		{
			return;
		}
		Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
	}
}

void UMainMenuWidget::JoinServer()
{
	if (!SelectedIndex.IsSet() || MainMenuInterface == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot join server, server hasn't been selected."));
		return;
	}
	MainMenuInterface->Join(SelectedIndex.GetValue());
}

void UMainMenuWidget::OpenJoinMenu()
{
	if (MenuSwitcher == nullptr || JoinMenu == nullptr || MainMenuInterface == nullptr)
	{
		return;
	}
	MenuSwitcher->SetActiveWidget(JoinMenu);
	MainMenuInterface->RefreshServerList();
}

void UMainMenuWidget::OpenMainMenu()
{
	if (MenuSwitcher == nullptr || JoinMenu == nullptr)
	{
		return;
	}
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenuWidget::QuitPressed()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	PlayerController->ConsoleCommand("quit");
}