// Copyright Telephone Studios. All Rights Reserved.


#include "UI/MapWidget.h"
#include "Components/Button.h"

UMapWidget::UMapWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);
	CloseButton = nullptr;
}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UMapWidget::Teardown);
	}

	if (SaveCurrentLocationButton)
	{
		SaveCurrentLocationButton->OnClicked.AddDynamic(this, &UMapWidget::SaveCurrentLocationButtonOnClicked);
	}
}

void UMapWidget::Setup()
{
	this->AddToViewport();

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeUIOnly InputData;
	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(InputData);

	this->SetFocus();

	// Update the coordinates
}

void UMapWidget::Teardown()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeGameOnly InputData;
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(InputData);

	// TODO broadcast this so we can set open widget to null in the map item
	this->RemoveFromParent();
}

FReply UMapWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		this->Teardown();
	}

	return FReply::Handled();
}

void UMapWidget::SaveCurrentLocationButtonOnClicked()
{
	// TODO add a saved location variable on the player
	// TODO add a saved location to the player save

	// Update the saved location on the owning player
	// Update the saved location text block

}
