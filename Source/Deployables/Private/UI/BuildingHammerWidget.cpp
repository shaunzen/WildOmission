// Copyright Telephone Studios. All Rights Reserved.


#include "UI/BuildingHammerWidget.h"
#include "Deployables/Deployable.h"
#include "Deployables/BuildingBlock.h"

void UBuildingHammerWidget::Setup(ADeployable* Deployable)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	this->AddToViewport();
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
	SetMouseCursorToCenter();
	this->SetFocus();
}

void UBuildingHammerWidget::Teardown()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
	}
	
	if (OnTeardown.IsBound())
	{
		OnTeardown.Broadcast();
	}
	this->RemoveFromParent();
}

FReply UBuildingHammerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	// TODO figure out which side we were leaning towards
	this->Teardown();

	return FReply::Handled();
}

void UBuildingHammerWidget::SetMouseCursorToCenter()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	PlayerController->SetMouseLocation(ViewportSizeX / 2, ViewportSizeY / 2);
}
