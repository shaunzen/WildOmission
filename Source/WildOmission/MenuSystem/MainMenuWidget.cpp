// Copyright Telephone Studios. All Rights Reserved.


#include "MainMenuWidget.h"
bool UMainMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (Success == false)
	{
		return false;
	}
	
	bIsFocusable = true;

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