// Copyright Telephone Studios. All Rights Reserved.

#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "InventoryWidget.h"
#include "SelectedItemWidget.h"
#include "VitalsWidget.h"
#include "Components/CanvasPanelSlot.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bInventoryOpen = false;
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UCanvasPanelSlot* SelectedItemSlot = Cast<UCanvasPanelSlot>(SelectedItem->Slot);
	if (PlayerController == nullptr || SelectedItemSlot == nullptr)
	{
		return;
	}
	double MouseX, MouseY;
	FVector2D ViewportSize;
	FVector2D A;
	FVector2D Destination;
	
	PlayerController->GetMousePosition(MouseX, MouseY);
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	// Convert mouse to normalized coordinates
	
	A = FVector2D(MouseX, MouseY) / ViewportSize;
	Destination = A * ViewportSize;
	UE_LOG(LogTemp, Warning, TEXT("Setting Selected Item to X: %f, Y: %f"), Destination.X, Destination.Y);
	SelectedItemSlot->SetPosition(Destination);
}

void UPlayerHUDWidget::ToggleInventory()
{
	bInventoryOpen = !bInventoryOpen;
	// Get player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	if (bInventoryOpen == false)
	{
		// Set input mode to game
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
		// Hide inventory menu
		BackgroundBorder->SetVisibility(ESlateVisibility::Hidden);
		Inventory->Close();
	}
	else
	{
		// Set input mode to ui
		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeData.SetHideCursorDuringCapture(false);
		//InputModeData.SetWidgetToFocus(Inventory->TakeWidget());
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
		// Show inventory menu
		BackgroundBorder->SetVisibility(ESlateVisibility::Visible);
		Inventory->Open();
	}
}

void UPlayerHUDWidget::SetInteractionPrompt(FString InString)
{
	InteractionPrompt->SetText(FText::FromString(InString));
}

void UPlayerHUDWidget::SetVitals(UVitalsComponent* InVitals)
{
	Vitals->Set(InVitals);
}

bool UPlayerHUDWidget::InventoryOpen()
{
	return bInventoryOpen;
}

UInventoryWidget* UPlayerHUDWidget::GetInventoryWidget()
{
	return Inventory;
}