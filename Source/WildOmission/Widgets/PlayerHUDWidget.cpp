// (c) 2023 Telephone Studios, all rights reserved.

#include "PlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "../ActorComponents/InventoryComponent.h"
#include "../ActorComponents/VitalsComponent.h"
#include "InventorySlotWidget.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (InventoryComponent == nullptr || InventorySlotClass == nullptr)
	{
		return;
	}

	for (const TPair<FName, int32>& Pair : *InventoryComponent->GetContent())
	{
		// this will be forgotten afterward
		UInventorySlotWidget* NewInventorySlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotClass);
		InventoryContentsWrapBox->AddChild(NewInventorySlot);
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