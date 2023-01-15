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
	CurrentItem.Name = FName();
	CurrentItem.Quantity = 0;
	bIsFull = false;
	SetItem(CurrentItem);
}

// Pass in Quantity of 0 to clear item from slot
void UInventorySlotWidget::SetItem(FSlotItem Item)
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
		bIsFull = CurrentItem.Quantity >= SlotItemData->StackSize;
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
	FSlotItem NewSlotItem;
	NewSlotItem.Clear();
	this->SetItem(NewSlotItem);
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
	if (CurrentItem.Quantity == 0)
	{
		return;
	}

	// Drag all of the items in this slot
	Owner->StartDragging(CurrentItem);
	// Clear this slot
	this->ClearItem();
}

void UInventorySlotWidget::LeftMouseDrop()
{
	// Get the item data about the item we are dragging
	if (!GetSelectedItemData())
	{
		return;
	}
	FSlotItem SelectedItem = *Owner->GetSelectedItem();

	// This slot is empty drop all the contents we are dragging into it
	if (CurrentItem.Quantity == 0)
	{
		this->SetItem(SelectedItem);
		Owner->EndDragging();
	}
	// This slot's item is the same as the selected item
	else if (CurrentItem.Name == SelectedItem.Name)
	{
		// If the amount we are trying to add is within stack size
		if ((CurrentItem.Quantity + SelectedItem.Quantity) <= GetSelectedItemData()->StackSize)
		{
			FSlotItem NewItem = CurrentItem;
			NewItem.Quantity += SelectedItem.Quantity;
			this->SetItem(NewItem);
			Owner->EndDragging();
		}
		// If the amount we are trying to add is over the stack size
		else if ((CurrentItem.Quantity + SelectedItem.Quantity) > GetSelectedItemData()->StackSize)
		{
			// Update the selected item
			FSlotItem NewSelection = SelectedItem;
			NewSelection.Quantity = (CurrentItem.Quantity + SelectedItem.Quantity) - GetSelectedItemData()->StackSize;
			Owner->StartDragging(NewSelection);
			
			// Update the slot item
			FSlotItem NewSlotItem = CurrentItem;
			NewSlotItem.Quantity = GetSelectedItemData()->StackSize;
			this->SetItem(NewSlotItem);
		}
	}
	// This slot's item is different to the one we are dragging
	else
	{
		FSlotItem OldSlotItem = CurrentItem;

		this->SetItem(SelectedItem);
		Owner->StartDragging(OldSlotItem);
	}
}

void UInventorySlotWidget::RightMouseDrag()
{
	// Early return if there is no item here
	if (CurrentItem.Quantity == 0)
	{
		return;
	}

	// If there is only one item
	if (CurrentItem.Quantity == 1)
	{
		Owner->StartDragging(CurrentItem);
		this->ClearItem();
		return;
	}
	
	// Split the quantity
	int32 HalfQuantity = CurrentItem.Quantity / 2;
	
	// Update the selection
	FSlotItem NewSelection = CurrentItem;
	NewSelection.Quantity = HalfQuantity;
	Owner->StartDragging(NewSelection);

	// Update the slot
	FSlotItem NewSlotItem = CurrentItem;
	NewSlotItem.Quantity -= HalfQuantity;
	this->SetItem(NewSlotItem);
}

void UInventorySlotWidget::RightMouseDrop()
{
	// Get the item data about the item we are dragging
	if (!GetSelectedItemData())
	{
		return;
	}
	FSlotItem SelectedItem = *Owner->GetSelectedItem();

	// Set the slot value to one and remove one from the selection if this slot has no item
	if (CurrentItem.Quantity == 0)
	{
		FSlotItem NewSlotItem = SelectedItem;
		NewSlotItem.Quantity = 1;
		this->SetItem(NewSlotItem);

		FSlotItem NewSelection = SelectedItem;
		NewSelection.Quantity -= 1;
		Owner->StartDragging(NewSelection);
	}
	// Add one item to this slot and remove one from the selection if this slot has the same item we are dragging and we are within stack size
	else if (CurrentItem.Name == SelectedItem.Name && (CurrentItem.Quantity + 1) <= GetSelectedItemData()->StackSize)
	{
		// Update the slot item
		FSlotItem NewSlotItem = CurrentItem;
		NewSlotItem.Quantity += 1;
		this->SetItem(NewSlotItem);

		// Update the selection
		FSlotItem NewSelection = SelectedItem;
		NewSelection.Quantity -= 1;
		Owner->StartDragging(NewSelection);
	}

	// Stop dragging if there are no more items to drop
	if (Owner->GetSelectedItem()->Quantity <= 0)
	{
		Owner->EndDragging();
	}
}

FItem* UInventorySlotWidget::GetSelectedItemData()
{
	FItem* SelectedItemData = nullptr;
	SelectedItemData = Owner->GetInventoryComponent()->GetItemData(Owner->GetSelectedItem()->Name);
	return SelectedItemData;
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

FSlotItem* UInventorySlotWidget::GetCurrentItem()
{
	return &CurrentItem;
}

bool UInventorySlotWidget::IsFull() const
{
	return bIsFull;
}