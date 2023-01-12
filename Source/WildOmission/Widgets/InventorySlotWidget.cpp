// Copyright Telephone Studios. All Rights Reserved.


#include "InventorySlotWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryWidget.h"
#include "WildOmission/ActorComponents/InventoryComponent.h"
#include "WildOmission/Characters/PlayerCharacter.h"
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
	bIsFull = false;
	bToolbarSlot = bInToolbarSlot;
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
			// Check if slot is full
			bIsFull = CurrentItemQuantity >= SlotItemData->StackSize;
		}
	}
	else
	{
		ItemIconBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		bIsFull = false;
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
			FItem* SelectedItemData = nullptr;
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				SelectedItemData = PlayerCharacter->GetInventoryComponent()->GetItemData(Owner->GetSelectedItem()->Name);
			}
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

				// Check if we add the amount will we go over the stack size
				
				if (SelectedItemData && (CurrentItemQuantity + Owner->GetSelectedItem()->Quantity) <= SelectedItemData->StackSize) // We went over the stack size
				{
					// Set the new quantity
					this->SetItem(Owner->GetSelectedItem()->Name, CurrentItemQuantity + Owner->GetSelectedItem()->Quantity);
					// Stop dragging
					Owner->EndDragging();
					
				}
				else if (SelectedItemData && (CurrentItemQuantity + Owner->GetSelectedItem()->Quantity) > SelectedItemData->StackSize) // We are under stack size
				{
					int32 NewSelectedQuantity;
					// Calculate leftover selected item quantity
					NewSelectedQuantity = (CurrentItemQuantity + Owner->GetSelectedItem()->Quantity) - SelectedItemData->StackSize;

					UE_LOG(LogTemp, Warning, TEXT("New Selected Quantity: %i"), NewSelectedQuantity);
					
					// Set the selected to new quantity
					Owner->StartDragging(Owner->GetSelectedItem()->Name, NewSelectedQuantity);
					// Set the quantity in this slot to the item stack size
					this->SetItem(Owner->GetSelectedItem()->Name, SelectedItemData->StackSize);
				}
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
			FItem* SelectedItemData = nullptr;
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				SelectedItemData = PlayerCharacter->GetInventoryComponent()->GetItemData(Owner->GetSelectedItem()->Name);
			}
			// This slot is empty
			if (CurrentItemQuantity == 0)
			{
				// Set the item in this slot to be the selected item with a quantity of one
				this->SetItem(Owner->GetSelectedItem()->Name, 1);
				// Decrement the quantity of selected item
				Owner->StartDragging(Owner->GetSelectedItem()->Name, Owner->GetSelectedItem()->Quantity - 1);
			}
			// This slot's item is the same as the selected item and currentquantity plus one is less than or equal to the stack size
			else if (SelectedItemData && CurrentItemName == Owner->GetSelectedItem()->Name && (CurrentItemQuantity + 1) <= SelectedItemData->StackSize)
			{
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
			// We have more than one item
			if (CurrentItemQuantity > 1)
			{
				// Get half of the quantity
				int32 HalfQuantity = CurrentItemQuantity / 2;
				// Remove that half from the slots quantity
				this->SetItem(CurrentItemName, CurrentItemQuantity - HalfQuantity);
				// Start dragging with the quantity of the half
				Owner->StartDragging(CurrentItemName, HalfQuantity);
			}
			else
			{
				Owner->StartDragging(CurrentItemName, CurrentItemQuantity);
				this->SetItem(FName(""), 0);
			}
		}
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

bool UInventorySlotWidget::IsFull() const
{
	return bIsFull;
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