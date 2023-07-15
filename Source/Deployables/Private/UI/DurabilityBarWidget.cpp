// Copyright Telephone Studios. All Rights Reserved.


#include "UI/DurabilityBarWidget.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/EquipComponent.h"
#include "Items/BuildingHammerItem.h"

void UDurabilityBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APawn* OwnerPawn = GetOwningPlayerPawn<APawn>();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	UEquipComponent* OwnerEquipComponent = OwnerPawn->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	float CurrentDurability = 0.0f;
	float MaxDurability = 0.0f;
	FString DurabilityActorName = FString();

	ABuildingHammerItem* HeldBuildingHammer = Cast<ABuildingHammerItem>(OwnerEquipComponent->GetEquipedItem());
	if (HeldBuildingHammer && HeldBuildingHammer->GetLookingAtItemDurability(CurrentDurability, MaxDurability, DurabilityActorName))
	{
		MainContainer->SetVisibility(ESlateVisibility::Visible);

		float Stability = 0.0f;
		if (HeldBuildingHammer->GetLookingAtItemStability(Stability))
		{
			FString StabilityString = FString::Printf(TEXT("%i% Stable"), FMath::RoundToInt32(Stability));
			StabilityTextBlock->SetText(FText::FromString(StabilityString));
		}
		else
		{
			StabilityTextBlock->SetText(FText::FromString(TEXT("")));
		}

		FString FractionString = FString::Printf(TEXT("%i/%i"), FMath::RoundToInt32(CurrentDurability), FMath::RoundToInt32(MaxDurability));
		DurabilityFractionTextBlock->SetText(FText::FromString(FractionString));

		DurabilityBar->SetPercent(CurrentDurability / MaxDurability);
		return;
	}

	MainContainer->SetVisibility(ESlateVisibility::Hidden);
}
