// Copyright Telephone Studios. All Rights Reserved.


#include "MainMenuWidget.h"
#include "ServerRowWidget.h"
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
	if (Success == false)
	{
		return false;
	}
	
	bIsFocusable = true;

	// TODO bind buttons

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