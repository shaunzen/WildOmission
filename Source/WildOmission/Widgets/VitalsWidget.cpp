// (c) 2023 Telephone Studios, all rights reserved.


#include "VitalsWidget.h"
#include "Components/ProgressBar.h"
#include "../ActorComponents/VitalsComponent.h"

void UVitalsWidget::Set(UVitalsComponent* InVitals)
{
	if (InVitals == nullptr || HealthBar == nullptr || ThirstBar == nullptr || HungerBar == nullptr)
	{
		return;
	}
	HealthBar->SetPercent(InVitals->GetHealth() / InVitals->GetMaxHealth());
	ThirstBar->SetPercent(InVitals->GetThirst() / InVitals->GetMaxThirst());
	HungerBar->SetPercent(InVitals->GetHunger() / InVitals->GetMaxHunger());
}