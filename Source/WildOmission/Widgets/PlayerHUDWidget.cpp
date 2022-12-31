// (c) 2023 Telephone Studios, all rights reserved.

#include "PlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "InventoryWidget.h"
#include "../ActorComponents/VitalsComponent.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bInventoryOpen = false;
}

void UPlayerHUDWidget::Setup(UInventoryComponent* InInventoryComponent)
{
	Inventory->Setup(InInventoryComponent);
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
		Inventory->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		// Set input mode to ui
		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeData.SetHideCursorDuringCapture(false);
		InputModeData.SetWidgetToFocus(Inventory->TakeWidget());
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
		// Show inventory menu
		Inventory->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerHUDWidget::SetInteractionPrompt(FString InString)
{
	InteractionPrompt->SetText(FText::FromString(InString));
}

void UPlayerHUDWidget::SetVitals(UVitalsComponent* InVitals)
{
	if (InVitals == nullptr || HealthBar == nullptr || ThirstBar == nullptr || HungerBar == nullptr)
	{
		return;
	}
	HealthBar->SetPercent(InVitals->GetHealth() / InVitals->GetMaxHealth());
	ThirstBar->SetPercent(InVitals->GetThirst() / InVitals->GetMaxThirst());
	HungerBar->SetPercent(InVitals->GetHunger() / InVitals->GetMaxHunger());
}