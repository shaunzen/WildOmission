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
		// Get the item data from the player character's inventory component
		FItem* SlotItemData = Owner->GetInventoryComponent()->GetItemData(Item.Name);
		if (SlotItemData == nullptr)
		{
			return;
		}

		// Set the item icon border to the item thumbnail
		ItemIconBorder->SetBrushFromMaterial(SlotItemData->Thumbnail);
		// Set the item icon color opaque white
		ItemIconBorder->SetBrushColor(FLinearColor::White);
	}
	else
	{
		ItemIconBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
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
	
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Owner->GetInventoryComponent()->Server_SlotInteraction(this->Index, true);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		Owner->GetInventoryComponent()->Server_SlotInteraction(this->Index, false);
	}

	Owner->Refresh();

	return FReply::Handled();
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