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

void UPlayerHUDWidget::ToggleInventory()
{
	bInventoryOpen = !bInventoryOpen;
	if (bInventoryOpen == false)
	{
		Inventory->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
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