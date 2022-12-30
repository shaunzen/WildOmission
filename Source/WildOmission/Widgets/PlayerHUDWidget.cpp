// (c) 2023 Telephone Studios, all rights reserved.

#include "PlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "../ActorComponents/VitalsComponent.h"

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