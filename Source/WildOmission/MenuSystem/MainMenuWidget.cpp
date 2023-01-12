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
	if (ServerRowWidgetBPClass.Class)
	{
		ServerRowWidgetClass = ServerRowWidgetBPClass.Class;
	}
}

bool UMainMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (Success == false
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

//****************************
// Menu Functions
//****************************
void UMainMenuWidget::HostServer()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		return;
	}
	FString ServerName = ServerHostName->Text.ToString();
	GameInstance->Host(ServerName);
}

void UMainMenuWidget::JoinServer()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (GameInstance == nullptr || SelectedIndex.IsSet() == false)
	{
		return;
	}
	GameInstance->Join(SelectedIndex.GetValue());
}

void UMainMenuWidget::OpenHostMenu()
{
	if (MenuSwitcher == nullptr)
	{
		return;
	}
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenuWidget::OpenJoinMenu()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (MenuSwitcher == nullptr || GameInstance == nullptr)
	{
		return;
	}
	MenuSwitcher->SetActiveWidget(JoinMenu);
	GameInstance->RefreshServerList();
}

void UMainMenuWidget::OpenMainMenu()
{
	if (MenuSwitcher == nullptr)
	{
		return;
	}
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenuWidget::QuitPressed()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	PlayerController->ConsoleCommand("quit");
}