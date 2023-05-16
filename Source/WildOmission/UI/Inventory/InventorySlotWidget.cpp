// Copyright Telephone Studios. All Rights Reserved.


#include "InventorySlotWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "InventoryWidget.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "WildOmission/UI/Inventory/HoveredItemNameTag.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Core/WildOmissionStatics.h"

void UInventorySlotWidget::Setup(UInventoryWidget* InOwner, const int32& InIndex)
{
	if (InOwner == nullptr || SlotBorder == nullptr || ItemIconBorder == nullptr || QuantityText == nullptr)
	{
		return;
	}

	Owner = InOwner;
	Index = InIndex;

	ClearItem();
}

// Pass in Quantity of 0 to clear item from slot
void UInventorySlotWidget::SetItem(const FInventoryItem& Item)
{
	FString QuantityString;
	if (Item.Quantity > 1)
	{
		QuantityString = FString::Printf(TEXT("x%i"), Item.Quantity);
	}
	else
	{
		QuantityString = FString("");
	}

	if (Item.Quantity != 0)
	{
		CurrentItemName = Item.Name;
		CurrentItemQuantity = Item.Quantity;

		FItemData* SlotItemData = UWildOmissionStatics::GetItemData(Item.Name);
		if (SlotItemData == nullptr)
		{
			return;
		}

		// Set the item icon border to the item thumbnail
		ItemIconBorder->SetBrushFromMaterial(SlotItemData->Thumbnail);
		// Set the item icon color opaque white
		ItemIconBorder->SetBrushColor(FLinearColor::White);

		if (SlotItemData->GetStat(FName("Durability")) > 0)
		{
			DurabilityBar->SetVisibility(ESlateVisibility::Visible);
			float Percent;
			Percent = (float)Item.GetStat(FName("Durability")) / (float)SlotItemData->GetStat(FName("Durability"));
			DurabilityBar->SetPercent(Percent);
		}
		else
		{
			DurabilityBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		CurrentItemName = FName();
		CurrentItemQuantity = 0;
		ItemIconBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		DurabilityBar->SetVisibility(ESlateVisibility::Hidden);
	}
	QuantityText->SetText(FText::FromString(QuantityString));
}

void UInventorySlotWidget::ClearItem()
{
	FInventoryItem BlankItem;

	this->SetItem(BlankItem);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn == nullptr || !Owner->GetParentHUD()->IsInventoryMenuOpen())
	{
		return FReply::Handled();
	}

	UInventoryManipulatorComponent* OwnerInventoryManipulatorComponent = PlayerPawn->FindComponentByClass<UInventoryManipulatorComponent>();
	if (OwnerInventoryManipulatorComponent == nullptr)
	{
		return FReply::Handled();
	}

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Owner->GetInventoryComponent()->SlotInteraction(this->Index, OwnerInventoryManipulatorComponent, true);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		Owner->GetInventoryComponent()->SlotInteraction(this->Index, OwnerInventoryManipulatorComponent, false);
	}

	HideHoveredItemNameTag();

	return FReply::Handled();
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	ShowHoveredItemNameTag();
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	HideHoveredItemNameTag();
}

int32 UInventorySlotWidget::GetIndex() const
{
	return Index;
}

void UInventorySlotWidget::SetSelected(bool InSelected)
{
	Selected = InSelected;
}

bool UInventorySlotWidget::IsSelected() const
{
	return Selected;
}

void UInventorySlotWidget::ShowHoveredItemNameTag()
{
	if (CurrentItemQuantity == 0)
	{
		return;
	}

	UPlayerHUDWidget* OwnerHUD = Owner->GetParentHUD();
	if (OwnerHUD == nullptr || OwnerHUD->SelectedItemVisible())
	{
		return;
	}

	UHoveredItemNameTag* HoveredItemNameTag = OwnerHUD->GetHoveredItemNameTag();
	if (HoveredItemNameTag == nullptr)
	{
		return;
	}
	FItemData* ItemData = UWildOmissionStatics::GetItemData(CurrentItemName);
	if (ItemData == nullptr)
	{
		return;
	}

	HoveredItemNameTag->Show(ItemData->DisplayName);
}

void UInventorySlotWidget::HideHoveredItemNameTag()
{
	UPlayerHUDWidget* OwnerHUD = Owner->GetParentHUD();
	if (OwnerHUD == nullptr)
	{
		return;
	}

	UHoveredItemNameTag* HoveredItemNameTag = OwnerHUD->GetHoveredItemNameTag();
	if (HoveredItemNameTag == nullptr)
	{
		return;
	}

	HoveredItemNameTag->Hide();
}