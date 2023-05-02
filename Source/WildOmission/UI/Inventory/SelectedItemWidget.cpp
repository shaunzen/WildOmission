// Copyright Telephone Studios. All Rights Reserved.


#include "SelectedItemWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Core/WildOmissionStatics.h"

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

void USelectedItemWidget::SetItem(UMaterialInstance* ItemIcon, const int32& Quantity)
{
	IconBorder->SetBrushFromMaterial(ItemIcon);
	QuantityText->SetText(FText::FromString(GetQuantityString(Quantity)));
}

void USelectedItemWidget::Show()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USelectedItemWidget::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

FString USelectedItemWidget::GetQuantityString(const int32& Quantity)
{
	FString QuantityString;
	
	if (Quantity > 1)
	{
		QuantityString = FString::Printf(TEXT("x%i"), Quantity);
	}
	else
	{
		QuantityString = FString("");
	}

	return QuantityString;
}

void USelectedItemWidget::Refresh(const FInventoryItem& SelectedItem)
{
	if (SelectedItem.Quantity > 0)
	{
		Show();
		FItemData* SelectedItemData = UWildOmissionStatics::GetItemData(SelectedItem.Name);
		SetItem(SelectedItemData->Thumbnail, SelectedItem.Quantity);
		
		FItemData* SlotItemData = UWildOmissionStatics::GetItemData(SelectedItem.Name);
		if (SlotItemData == nullptr)
		{
			return;
		}
		
		if (SlotItemData->GetStat(FName("Durability")) > 0)
		{
			DurabilityBar->SetVisibility(ESlateVisibility::Visible);
			float Percent;
			Percent = (float)SelectedItem.GetStat(FName("Durability")) / (float)SlotItemData->GetStat(FName("Durability"));
			DurabilityBar->SetPercent(Percent);
		}
		else
		{
			DurabilityBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		Hide();
	}
}