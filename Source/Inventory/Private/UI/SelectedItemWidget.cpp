// Copyright Telephone Studios. All Rights Reserved.


#include "UI/SelectedItemWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"

void USelectedItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn == nullptr)
	{
		return;
	}

	UInventoryManipulatorComponent* PlayerInventoryManipulator = PlayerPawn->FindComponentByClass<UInventoryManipulatorComponent>();
	if (PlayerInventoryManipulator == nullptr)
	{
		return;
	}

	PlayerInventoryManipulator->OnSelectionChanged.AddDynamic(this, &USelectedItemWidget::Refresh);
}


void USelectedItemWidget::Show()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USelectedItemWidget::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void USelectedItemWidget::Refresh(const FInventoryItem& SelectedItem)
{
	FString QuantityString;

	if (SelectedItem.Quantity > 1)
	{
		QuantityString = FString::Printf(TEXT("x%i"), SelectedItem.Quantity);
	}
	else
	{
		QuantityString = FString("");
	}

	if (SelectedItem.Quantity > 0)
	{
		Show();
		
		FItemData* SlotItemData = UInventoryComponent::GetItemData(SelectedItem.Name);
		if (SlotItemData == nullptr)
		{
			return;
		}
		
		IconBorder->SetBrushFromMaterial(SlotItemData->Thumbnail);
		
		if (SlotItemData->GetStat(FName("Durability")) > 0)
		{
			DurabilityBar->SetVisibility(ESlateVisibility::Visible);
			float Percent;
			Percent = (float)SelectedItem.GetStat(FName("Durability")) / (float)SlotItemData->GetStat(FName("Durability"));
			DurabilityBar->SetPercent(Percent);
			DurabilityBar->SetFillColorAndOpacity(FMath::Lerp(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), Percent));
		}
		else
		{
			DurabilityBar->SetVisibility(ESlateVisibility::Hidden);
		}
		
		if (SlotItemData->GetStat(TEXT("MaxAmmo")) != INDEX_NONE)
		{
			QuantityString = FString::Printf(TEXT("%i/%i"), SelectedItem.GetStat(TEXT("CurrentAmmo")), SelectedItem.GetStat(TEXT("MaxAmmo")));
		}

		QuantityText->SetText(FText::FromString(QuantityString));
	}
	else
	{
		Hide();
	}
}