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
	if (InOwner == nullptr || SlotButton == nullptr || ItemIconBorder == nullptr || QuantityText == nullptr)
	{
		return;
	}

	Owner = InOwner;
	CurrentItemName = FName("");
	CurrentItemQuantity = 0;
	bToolbarSlot = bInToolbarSlot;
	SetItem(CurrentItemName, CurrentItemQuantity);
	
	SlotButton->OnPressed.AddDynamic(this, &UInventorySlotWidget::OnPressed);
}

// Pass in Quantity of 0 to clear item from slot
void UInventorySlotWidget::SetItem(FName ItemName, int32 ItemQuantity)
{
	CurrentItemName = ItemName;
	CurrentItemQuantity = ItemQuantity;

	FString QuantityString;
	if (CurrentItemQuantity > 1)
	{
		QuantityString = FString::Printf(TEXT("%i"), CurrentItemQuantity);
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

void UInventorySlotWidget::OnPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Slot pressed."));
	
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
	if (Owner->Dragging())
	{
		if (CurrentItemQuantity == 0)
		{
			this->SetItem(Owner->GetSelectedItem()->Name, Owner->GetSelectedItem()->Quantity);
			Owner->EndDragging();
		}
		else if (CurrentItemName == Owner->GetSelectedItem()->Name)
		{
			int32 NewQuantity;
			NewQuantity = CurrentItemQuantity + Owner->GetSelectedItem()->Quantity;
			this->SetItem(Owner->GetSelectedItem()->Name, NewQuantity);
			Owner->EndDragging();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("It's something else entirely."));
		}
	}
	else
	{
		if (CurrentItemQuantity > 0)
		{
			Owner->StartDragging(CurrentItemName, CurrentItemQuantity);
			this->SetItem(FName(""), 0);
		}
	}
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