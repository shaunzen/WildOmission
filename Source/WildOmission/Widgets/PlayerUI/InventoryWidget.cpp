// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "SelectedItemWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/CanvasPanel.h"
#include "UObject/ConstructorHelpers.h"
#include "WildOmission/Components/InventoryComponent.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UInventorySlotWidget> InventorySlotBPWidgetClass(TEXT("/Game/WildOmission/Blueprints/Widgets/PlayerUI/WBP_InventorySlot"));
	if (InventorySlotBPWidgetClass.Class == nullptr)
	{
		return;
	}

	SlotWidgetClass = InventorySlotBPWidgetClass.Class;
}

void UInventoryWidget::SetSelectedItemWidget(USelectedItemWidget* InWidget)
{
	SelectedItemWidget = InWidget;
	SelectedItemWidget->Hide();
}

void UInventoryWidget::Setup(UInventoryComponent* InInventoryComponent)
{
	// Check if inventory and slot are valid pointers
	if (InInventoryComponent == nullptr || SlotWidgetClass == nullptr)
	{
		return;
	}
	
	InventoryComponent = InInventoryComponent;

	CreateSlots(true);

	// Set default visibility
	InventoryName->SetVisibility(ESlateVisibility::Hidden);
	InventoryGridPanel->SetVisibility(ESlateVisibility::Hidden);
	ToolbarGridPanel->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryWidget::CreateSlots(bool CreateToolbar)
{
	if (CreateToolbar)
	{
		CreateToolbarSlots();
	}
	CreateInventorySlots();
}

void UInventoryWidget::CreateToolbarSlots()
{
	for (uint8 i = 0; i < 6; ++i)
	{
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		NewSlot->Setup(this, i);
		
		ToolbarGridPanel->AddChild(NewSlot);
		
		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(NewSlot->Slot);

		GridSlot->SetColumn(i);
		GridSlot->SetRow(0);

		Slots.Add(NewSlot);
	}
}

void UInventoryWidget::CreateInventorySlots()
{
	uint8 Column = 0;
	uint8 Row = 0;

	for (uint8 i = 0; i < 24; ++i)
	{
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		NewSlot->Setup(this, i + 6);
	
		InventoryGridPanel->AddChild(NewSlot);

		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(NewSlot->Slot);
		
		GridSlot->SetColumn(Column);
		GridSlot->SetRow(Row);

		Slots.Add(NewSlot);

		++Column;
		if (Column > 5)
		{
			Column = 0;
			++Row;
		}
	}
}

void UInventoryWidget::Open()
{
	InventoryName->SetVisibility(ESlateVisibility::Visible);
	InventoryGridPanel->SetVisibility(ESlateVisibility::Visible);

	Refresh();
}

void UInventoryWidget::Close()
{
	InventoryName->SetVisibility(ESlateVisibility::Hidden);
	InventoryGridPanel->SetVisibility(ESlateVisibility::Hidden);

	InventoryComponent->StopDragging(true);
	
	Refresh();
}

void UInventoryWidget::Refresh()
{
	for (UInventorySlotWidget* InventorySlot : Slots)
	{
		InventorySlot->SetItem(InventoryComponent->GetSlots()[InventorySlot->GetIndex()].Item);
		
		// Check if it is a selected slot
		InventorySlot->SetSelected(InventorySlot->GetIndex() == InventoryComponent->GetToolbarSelectionIndex());
	}

	if (InventoryComponent->IsDragging())
	{
		SelectedItemWidget->Show();
		FItem* SelectedItemData = InventoryComponent->GetItemData(InventoryComponent->GetSelectedItem()->Name);
		SelectedItemWidget->SetItem(SelectedItemData->Thumbnail, InventoryComponent->GetSelectedItem()->Quantity);
	}
	else
	{
		SelectedItemWidget->Hide();
	}
}

UInventoryComponent* UInventoryWidget::GetInventoryComponent()
{
	return InventoryComponent;
}