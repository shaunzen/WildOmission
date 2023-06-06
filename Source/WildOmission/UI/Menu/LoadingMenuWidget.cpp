// Copyright Telephone Studios. All Rights Reserved.


#include "LoadingMenuWidget.h"
#include "Components/TextBlock.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"

void ULoadingMenuWidget::Setup()
{
	this->AddToViewport();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	PlayerController->SetMouseLocation(ViewportSizeX / 2, ViewportSizeY / 2);
}

void ULoadingMenuWidget::Teardown()
{
	this->RemoveFromParent();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void ULoadingMenuWidget::SetLoadingTitle(const FString& Title)
{
	TitleTextBlock->SetText(FText::FromString(Title));
}

void ULoadingMenuWidget::SetLoadingStep(const FString& StepName, const int32& StepPercent)
{
	FString StepText = FString();
	
	if (StepPercent == -1)
	{
		StepText = StepName;
	}
	else
	{
		StepText = FString::Printf(TEXT("%s: %i%"), *StepName, StepPercent);
	}

	SubtitleTextBlock->SetText(FText::FromString(StepText));
}