// Copyright Telephone Studios. All Rights Reserved.


#include "InventorySlotWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryWidget.h"
#include "WildOmission/Components/InventoryComponent.h"

void UInventorySlotWidget::Setup(UInventoryWidget* InOwner, const int32& InIndex)
{
	if (InOwner == nullptr || SlotBorder == nullptr || ItemIconBorder == nullptr || QuantityText == nullptr)
	{
		return;
	}

	Owner = InOwner;
	Index = InIndex;
	CurrentItem.Name = FName();
	CurrentItem.Quantity = 0;
	SetItem(CurrentItem);
}

// Pass in Quantity of 0 to clear item from slot
void UInventorySlotWidget::SetItem(const FInventoryItem& Item)
{
	CurrentItem = Item;

	FString QuantityString;
	if (CurrentItem.Quantity > 1)
	{
		QuantityString = FString::Printf(TEXT("x%i"), CurrentItem.Quantity);
	}
	else
	{
		QuantityString = FString("");
	}

	if (CurrentItem.Quantity != 0)
	{
		// Get the item data from the player character's inventory component
		FItem* SlotItemData = Owner->GetInventoryComponent()->GetItemData(CurrentItem.Name);
		if (SlotItemData == nullptr)
		{
			return;
		}

		// Set the item icon border to the item thumbnail
		ItemIconBorder->SetBrushFromTexture(SlotItemData->Thumbnail);
		// Set the item icon color opaque white
		ItemIconBorder->SetBrushColor(FLinearColor::White);
		// Check if slot is full
	}
	else
	{
		ItemIconBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		
	}
	QuantityText->SetText(FText::FromString(QuantityString));
}

void UInventorySlotWidget::ClearItem()
{
	FInventoryItem NewSlotItem;
	NewSlotItem.Clear();
	this->SetItem(NewSlotItem);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Owner->GetInventoryComponent()->SlotInteraction(this->Index, true);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		Owner->GetInventoryComponent()->SlotInteraction(this->Index, false);
	}

	return FReply::Handled();
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	// TODO set slot color a lighter shade
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	// TODO set slot color to default shade
}

FInventoryItem* UInventorySlotWidget::GetCurrentItem()
{
	return &CurrentItem;
}