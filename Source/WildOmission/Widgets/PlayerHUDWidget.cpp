// Copyright Telephone Studios. All Rights Reserved.

#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryWidget.h"
#include "SelectedItemWidget.h"
#include "VitalsWidget.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bInventoryOpen = false;
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BackgroundBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("BackgroundMouseButtonDown"));
	Inventory->SetSelectedItemWidget(SelectedItem);
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdateSelectedItemLocation();
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

void UPlayerHUDWidget::UpdateSelectedItemLocation()
{	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UCanvasPanelSlot* SelectedItemSlot = Cast<UCanvasPanelSlot>(SelectedItem->Slot);
	if (PlayerController == nullptr || SelectedItemSlot == nullptr || GEngine->GameViewport == 0)
	{
		return;
	}

	// Get mouse position
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

	// Get viewport size
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	// Normalize the mouse position
	FVector2D NormalizedMousePosition;
	NormalizedMousePosition = MousePosition / ViewportSize;

	// Create anchor from destination
	FAnchors NewAnchor;
	NewAnchor.Minimum.X = NormalizedMousePosition.X;
	NewAnchor.Minimum.Y = NormalizedMousePosition.Y;
	NewAnchor.Maximum.X = NormalizedMousePosition.X;
	NewAnchor.Maximum.Y = NormalizedMousePosition.Y;

	// Update slot anchor and position
	SelectedItemSlot->SetAnchors(NewAnchor);
	SelectedItemSlot->SetPosition(FVector2D::ZeroVector);
}

void UPlayerHUDWidget::BackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (Inventory->Dragging())
	{
		if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			Inventory->DropSelectedItem(true);
		}
		else if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			Inventory->DropSelectedItem(false);
		}
	}
}