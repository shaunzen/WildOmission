// Copyright Telephone Studios. All Rights Reserved.


#include "InventorySlotWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryWidget.h"
#include "WildOmission/ActorComponents/InventoryComponent.h"

void UInventorySlotWidget::Setup(UInventoryWidget* InOwner)
{
	if (InOwner == nullptr || SlotBorder == nullptr || ItemIconBorder == nullptr || QuantityText == nullptr)
	{
		return;
	}

	Owner = InOwner;
	CurrentItemName = FName("");
	CurrentItemQuantity = 0;
	bIsFull = false;
	SetItem(CurrentItemName, CurrentItemQuantity);
}

// Pass in Quantity of 0 to clear item from slot
void UInventorySlotWidget::SetItem(FName ItemName, int32 ItemQuantity)
{
	CurrentItemName = ItemName;
	CurrentItemQuantity = ItemQuantity;

	FString QuantityString;
	if (CurrentItemQuantity > 1)
	{
		QuantityString = FString::Printf(TEXT("x%i"), CurrentItemQuantity);
	}
	else
	{
		QuantityString = FString("");
	}

	if (CurrentItemQuantity != 0)
	{
		// Get the item data from the player character's inventory component
		FItem* SlotItemData = Owner->GetInventoryComponent()->GetItemData(CurrentItemName);
		if (SlotItemData == nullptr)
		{
			return;
		}

		// Set the item icon border to the item thumbnail
		ItemIconBorder->SetBrushFromTexture(SlotItemData->Thumbnail);
		// Set the item icon color opaque white
		ItemIconBorder->SetBrushColor(FLinearColor::White);
		// Check if slot is full
		bIsFull = CurrentItemQuantity >= SlotItemData->StackSize;
	}
	else
	{
		ItemIconBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		bIsFull = false;
	}
	QuantityText->SetText(FText::FromString(QuantityString));
}

void UInventorySlotWidget::ClearItem()
{
	this->SetItem(FName(""), 0);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (!Owner->Dragging())
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			LeftMouseDrag();
		}
		else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			RightMouseDrag();
		}
	}
	else
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			LeftMouseDrop();
		}
		else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			RightMouseDrop();
		}
	}
	return FReply::Handled();
}

//*
// Drag and drop functions
//*
void UInventorySlotWidget::LeftMouseDrag()
{
	// Return early if there is no item here
	if (CurrentItemQuantity == 0)
	{
		return;
	}

	// Drag all of the items in this slot
	Owner->StartDragging(CurrentItemName, CurrentItemQuantity);
	// Clear this slot
	this->ClearItem();
}

void UInventorySlotWidget::LeftMouseDrop()
{
	// Get the item data about the item we are dragging
	FItem* SelectedItemData = nullptr;
	SelectedItemData = Owner->GetInventoryComponent()->GetItemData(Owner->GetSelectedItem()->Name);
	if (SelectedItemData == nullptr)
	{
		return;
	}

	// This slot is empty drop all the contents we are dragging into it
	if (CurrentItemQuantity == 0)
	{
		this->SetItem(Owner->GetSelectedItem()->Name, Owner->GetSelectedItem()->Quantity);
		Owner->EndDragging();
	}
	// This slot's item is the same as the selected item TODO denest
	else if (CurrentItemName == Owner->GetSelectedItem()->Name)
	{
		// If the amount we are trying to add is within stack size
		if ((CurrentItemQuantity + Owner->GetSelectedItem()->Quantity) <= SelectedItemData->StackSize)
		{
			this->SetItem(Owner->GetSelectedItem()->Name, CurrentItemQuantity + Owner->GetSelectedItem()->Quantity);
			Owner->EndDragging();
		}
		// If the amount we are trying to add is over the stack size
		else if ((CurrentItemQuantity + Owner->GetSelectedItem()->Quantity) > SelectedItemData->StackSize)
		{
			// Calculate leftover selected item quantity
			int32 NewSelectedQuantity;
			NewSelectedQuantity = (CurrentItemQuantity + Owner->GetSelectedItem()->Quantity) - SelectedItemData->StackSize;

			Owner->StartDragging(Owner->GetSelectedItem()->Name, NewSelectedQuantity);
			this->SetItem(Owner->GetSelectedItem()->Name, SelectedItemData->StackSize);
		}
	}
	// This slot's item is different to the one we are dragging
	else
	{
		// TODO swap
		UE_LOG(LogTemp, Warning, TEXT("It's something else entirely."));
	}
}

void UInventorySlotWidget::RightMouseDrag()
{
	// Early return if there is no item here
	if (CurrentItemQuantity == 0)
	{
		return;
	}

	// If there is only one item
	if (CurrentItemQuantity == 1)
	{
		Owner->StartDragging(CurrentItemName, CurrentItemQuantity);
		this->ClearItem();
		return;
	}
	
	// Split the quantity
	int32 HalfQuantity = CurrentItemQuantity / 2;
	Owner->StartDragging(CurrentItemName, HalfQuantity);
	this->SetItem(CurrentItemName, CurrentItemQuantity - HalfQuantity);
}

void UInventorySlotWidget::RightMouseDrop()
{
	// Get the item data about the item we are dragging
	FItem* SelectedItemData = nullptr;
	SelectedItemData = Owner->GetInventoryComponent()->GetItemData(Owner->GetSelectedItem()->Name);
	if (SelectedItemData == nullptr)
	{
		return;
	}

	// Set the slot value to one and remove one from the drag if this slot has no item
	if (CurrentItemQuantity == 0)
	{
		this->SetItem(Owner->GetSelectedItem()->Name, 1);
		Owner->StartDragging(Owner->GetSelectedItem()->Name, Owner->GetSelectedItem()->Quantity - 1);
	}
	// Add one item to this slot and remove one from the drag if this slot has the same item we are dragging and we are within stack size
	else if (CurrentItemName == Owner->GetSelectedItem()->Name && (CurrentItemQuantity + 1) <= SelectedItemData->StackSize)
	{
		this->SetItem(Owner->GetSelectedItem()->Name, CurrentItemQuantity + 1);
		Owner->StartDragging(Owner->GetSelectedItem()->Name, Owner->GetSelectedItem()->Quantity - 1);
	}

	// Stop dragging if there are no more items to drop
	if (Owner->GetSelectedItem()->Quantity <= 0)
	{
		Owner->EndDragging();
	}
}

//*
// End drag and drop functions
//*

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

FName UInventorySlotWidget::GetCurrentItemName()
{
	return CurrentItemName;
}

int32 UInventorySlotWidget::GetCurrentItemQuantity()
{
	return CurrentItemQuantity;
}

bool UInventorySlotWidget::IsFull() const
{
	return bIsFull;
}