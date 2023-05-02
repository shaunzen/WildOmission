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
	ConstructorHelpers::FClassFinder<UInventorySlotWidget> InventorySlotBPWidgetClass(TEXT("/Game/WildOmission/UI/Inventory/WBP_InventorySlot"));
	if (InventorySlotBPWidgetClass.Class == nullptr)
	{
		return;
	}

	SlotWidgetClass = InventorySlotBPWidgetClass.Class;
}

void UInventoryWidget::Setup(UPlayerHUDWidget* InParentHUD, UInventoryComponent* InInventoryComponent)
{
	// Check if inventory and slot are valid pointers
	if (InInventoryComponent == nullptr || SlotWidgetClass == nullptr)
	{
		return;
	}
	ParentPlayerHUD = InParentHUD;

	InventoryComponent = InInventoryComponent;
	
	// Set default visibility
	InventoryName->SetVisibility(ESlateVisibility::Hidden);
	InventoryGridPanel->SetVisibility(ESlateVisibility::Hidden);

	InInventoryComponent->OnUpdate.AddDynamic(this, &UInventoryWidget::Refresh);
}

void UInventoryWidget::CreateSlots()
{
	uint8 Column = 0;
	uint8 Row = 0;

	for (uint8 i = 0; i < InventoryComponent->GetSlotCount(); ++i)
	{
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		NewSlot->Setup(this, i);
	
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
	
	Refresh();
}

void UInventoryWidget::Refresh()
{
	if (InventoryComponent->GetSlots().Num() == 0)
	{
		return;
	}

	for (UInventorySlotWidget* InventorySlot : Slots)
	{
		InventorySlot->SetItem(InventoryComponent->GetSlot(InventorySlot->GetIndex())->Item);
		InventorySlot->SetSelected(false);
	}
}

void UInventoryWidget::RefreshSlot(const int32& SlotIndex)
{
	Slots[SlotIndex]->SetItem(InventoryComponent->GetSlot(SlotIndex)->Item);
}

UPlayerHUDWidget* UInventoryWidget::GetParentHUD()
{
	return ParentPlayerHUD;
}

UInventoryComponent* UInventoryWidget::GetInventoryComponent()
{
	return InventoryComponent;
}