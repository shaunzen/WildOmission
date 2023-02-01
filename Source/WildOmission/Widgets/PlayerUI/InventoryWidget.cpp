// Copyright Telephone Studios. All Rights Reserved.


#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "SelectedItemWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/CanvasPanel.h"
#include "WildOmission/Components/InventoryComponent.h"

void UInventoryWidget::SetSelectedItemWidget(USelectedItemWidget* InWidget)
{
	SelectedItemWidget = InWidget;
	SelectedItemWidget->Hide();
}

void UInventoryWidget::SetComponent(UInventoryComponent* InInventoryComponent)
{
	// Check if inventory and slot are valid pointers
	if (InInventoryComponent == nullptr || InventorySlotWidgetClass == nullptr)
	{
		return;
	}
	
	InventoryComponent = InInventoryComponent;

	CreateSlots();

	// Set default visibility
	InventoryName->SetVisibility(ESlateVisibility::Hidden);
	InventoryGridPanel->SetVisibility(ESlateVisibility::Hidden);
	ToolbarGridPanel->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryWidget::CreateSlots()
{
	CreateToolbarSlots();
	CreateInventorySlots();
}

void UInventoryWidget::CreateToolbarSlots()
{
	for (uint8 i = 0; i < 6; ++i)
	{
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		NewSlot->Setup(this, i);
		
		ToolbarGridPanel->AddChild(NewSlot);
		
		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(NewSlot->Slot);

		GridSlot->SetColumn(i);
		GridSlot->SetRow(0);

		InventorySlots.Add(NewSlot);
	}
}

void UInventoryWidget::CreateInventorySlots()
{
	uint8 Column = 0;
	uint8 Row = 0;

	for (uint8 i = 0; i < 24; ++i)
	{
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		NewSlot->Setup(this, i + 6);
	
		InventoryGridPanel->AddChild(NewSlot);

		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(NewSlot->Slot);
		
		GridSlot->SetColumn(Column);
		GridSlot->SetRow(Row);

		InventorySlots.Add(NewSlot);

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
}

void UInventoryWidget::Close()
{
	InventoryName->SetVisibility(ESlateVisibility::Hidden);
	InventoryGridPanel->SetVisibility(ESlateVisibility::Hidden);
}

UInventoryComponent* UInventoryWidget::GetInventoryComponent()
{
	return InventoryComponent;
}