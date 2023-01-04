// Copyright Telephone Studios. All Rights Reserved.


#include "InventorySlotWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryWidget.h"
#include "../ActorComponents/InventoryComponent.h"
#include "../Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UInventorySlotWidget::Setup(UInventoryWidget* InOwner, bool bInToolbarSlot)
{
	if (InOwner == nullptr || SlotBorder == nullptr || ItemIconBorder == nullptr || QuantityText == nullptr)
	{
		return;
	}

	Owner = InOwner;
	CurrentItemName = FName("");
	CurrentItemQuantity = 0;
	bToolbarSlot = bInToolbarSlot;
	SetItem(CurrentItemName, CurrentItemQuantity);
	
	//SlotButton->OnPressed.AddDynamic(this, &UInventorySlotWidget::OnPressed);
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
		// Get the player character
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			// Get the item data from the player character's inventory component
			FItem* SlotItemData = PlayerCharacter->GetInventoryComponent()->GetItemData(CurrentItemName);
			if (SlotItemData == nullptr)
			{
				return;
			}
			// Set the item icon border to the item thumbnail
			ItemIconBorder->SetBrushFromTexture(SlotItemData->Thumbnail);
			// Set the item icon color opaque white
			ItemIconBorder->SetBrushColor(FLinearColor::White);
		}
	}
	else
	{
		ItemIconBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
	}
	QuantityText->SetText(FText::FromString(QuantityString));
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// TODO stack size implementation
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// We are currently dragging
		if (Owner->Dragging())
		{
			// This slot is empty
			if (CurrentItemQuantity == 0)
			{
				// Set this slot's item and quantity to the selected
				this->SetItem(Owner->GetSelectedItem()->Name, Owner->GetSelectedItem()->Quantity);
				// Stop dragging
				Owner->EndDragging();
			}
			// This slot's item is the same as the selected item
			else if (CurrentItemName == Owner->GetSelectedItem()->Name)
			{
				// Add the selected quantity to the current slot quantity
				int32 NewQuantity;
				NewQuantity = CurrentItemQuantity + Owner->GetSelectedItem()->Quantity;
				// Set the new quantity
				this->SetItem(Owner->GetSelectedItem()->Name, NewQuantity);
				// Stop dragging
				Owner->EndDragging();
			}
			else // This slots item is different to the selected item
			{
				UE_LOG(LogTemp, Warning, TEXT("It's something else entirely."));
			}
		}
		else // We are not currently dragging
		{
			// There is an item in this slot
			if (CurrentItemQuantity > 0)
			{
				// Start dragging all items in this slot
				Owner->StartDragging(CurrentItemName, CurrentItemQuantity);
				// Clear this slot
				this->SetItem(FName(""), 0);
			}
		}
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		// We are currently dragging
		if (Owner->Dragging())
		{
			// This slot is empty
			if (CurrentItemQuantity == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Current item was empty set to selected item with value of 1"));
				// Set the item in this slot to be the selected item with a quantity of one
				this->SetItem(Owner->GetSelectedItem()->Name, 1);
				// Decrement the quantity of selected item
				Owner->StartDragging(Owner->GetSelectedItem()->Name, Owner->GetSelectedItem()->Quantity - 1);
			}
			// This slot's item is the same as the selected item and currentquantity plus one is less than the stack size
			else if (CurrentItemName == Owner->GetSelectedItem()->Name)
			{
				UE_LOG(LogTemp, Warning, TEXT("Current item was the same, set the quantity to %i"), CurrentItemQuantity + 1);
				// Increment the slot quantity
				this->SetItem(Owner->GetSelectedItem()->Name, CurrentItemQuantity + 1);
				// Decrement to quantity of selected item
				Owner->StartDragging(Owner->GetSelectedItem()->Name, Owner->GetSelectedItem()->Quantity - 1);
			}
			// If quantity of selected item is = 0
			if (Owner->GetSelectedItem()->Quantity <= 0)
			{
				// Stop dragging
				Owner->EndDragging();
			}
		}
		else // We are not dragging
		{
			// TODO only take half
		}
	}
	// TODO middle click
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

void UInventorySlotWidget::OnPressed()
{
	// is owner currently dragging
	// yes
		// we dont have an item
			// set the current item and quantity to the selected one
			// clear selected
		// we have the same item as the one being dragged
			// add the selected quantity to our quantity
			// clear slected
		// we have an item but its different to the one being dragged
			// swap
	// no
		// we have an item
			// set selected item and quantity to the one in this slot
			// clear the item and quantity in this slot
	
}

FName UInventorySlotWidget::GetCurrentItemName()
{
	return CurrentItemName;
}

int32 UInventorySlotWidget::GetCurrentItemQuantity()
{
	return CurrentItemQuantity;
}

/*
	//************************************
	// Amount Text
	FString QuantityString;
	
	if (Quantity > 1)
	{
		// Only Set the string to a number if there is more than one
		QuantityString = FString::Printf(TEXT("%i"), Quantity);
	}
	else
	{
		QuantityString = FString("");
	}
	QuantityText->SetText(FText::FromString(QuantityString));

	//************************************
	// Image Icon
	FItem* SlotItemData = OwnerInventoryComponent->GetItemData(ItemName);
	
	if (SlotItemData == nullptr)
	{
		return;
	}

	SlotImageBorder->SetBrushFromTexture(SlotItemData->Thumbnail);
	SlotImageBorder->SetBrushColor(FLinearColor::White);
*/