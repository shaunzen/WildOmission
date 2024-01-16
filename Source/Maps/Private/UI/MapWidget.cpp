// Copyright Telephone Studios. All Rights Reserved.


#include "UI/MapWidget.h"

UMapWidget::UMapWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	CloseButton = nullptr;
}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMapWidget::Setup()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeUIOnly InputData;
	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(InputData);
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
