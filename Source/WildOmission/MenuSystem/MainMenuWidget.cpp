// Copyright Telephone Studios. All Rights Reserved.


#include "MainMenuWidget.h"
#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
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
	
	bIsFocusable = true;

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
	UE_LOG(LogTemp, Display, TEXT("Setting Up Main Menu Widget"));
	this->AddToViewport();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(this->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
		UE_LOG(LogTemp, Display, TEXT("Finished Setting Up Main Menu Widget"));
	}
}

void UMainMenuWidget::Teardown()
{
	this->RemoveFromViewport();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
		UE_LOG(LogTemp, Display, TEXT("Removed Main Menu from viewport"));
	}
}

void UMainMenuWidget::SetServerList(TArray<FServerData> ServerNames)
{

}

void UMainMenuWidget::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenuWidget::UpdateChildren()
{

}

//****************************
// Menu Functions
//****************************
void UMainMenuWidget::HostServer()
{
	// TODO Game instance host server
}

void UMainMenuWidget::JoinServer()
{
	// TODO Game instance join server
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
	if (MenuSwitcher == nullptr)
	{
		return;
	}
	MenuSwitcher->SetActiveWidget(JoinMenu);
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