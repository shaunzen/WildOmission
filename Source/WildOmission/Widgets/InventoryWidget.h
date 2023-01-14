// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UWrapBox;
class UBorder;
class USelectedItemWidget;
class UInventoryComponent;

UCLASS()
class WILDOMISSION_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Setup
	void SetSelectedItemWidget(USelectedItemWidget* InWidget);
	void SetComponent(UInventoryComponent* InInventoryComponent);

	bool AddItem(FName ItemName, int32 Quantity, int32& Remaining);

	void Open();
	void Close();

	// Dragging
	void StartDragging(FSlotItem Item);
	void EndDragging();
	bool Dragging() const;
	
	FSlotItem* GetSelectedItem();
	
	UInventoryComponent* GetInventoryComponent();

private:
	// Slots
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;
	UPROPERTY()
	TArray<UInventorySlotWidget*> InventorySlots;
	UPROPERTY()
	USelectedItemWidget* SelectedItemWidget;

	FSlotItem SelectedItem;
	bool bCurrentlyDragging;

	// Bind Widget Elements
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InventoryName;
	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;
	UPROPERTY(meta = (BindWidget))
	UWrapBox* ToolbarWrapBox;
	
	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	void CreateInventorySlots();

	bool AddItemToPopulatedSlot(const FName& ItemName, struct FItem* ItemData, int32& QuantityToAdd);
	bool AddItemToEmptySlot(const FName& ItemName, struct FItem* ItemData, int32& QuantityToAdd);

};
