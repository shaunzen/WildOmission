// (c) 2023 Telephone Studios, all rights reserved.

#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "InventoryWidget.h"
#include "VitalsWidget.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bInventoryOpen = false;
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