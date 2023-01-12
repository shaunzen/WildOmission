// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UWrapBox;
class UBorder;
class UInventorySlotWidget;
class USelectedItemWidget;
class UInventoryComponent;

USTRUCT()
struct FSelectedItem
{
	GENERATED_BODY()

	FName Name;
	int32 Quantity;
	FSelectedItem(FName InName = FName(""), int32 InQuantity = 0)
	{
		Name = InName;
		Quantity = InQuantity;
	}
};

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
	void StartDragging(FName ItemName, int32 Quantity);
	void EndDragging();
	bool Dragging() const;
	
	FSelectedItem* GetSelectedItem();
	
	UInventoryComponent* GetInventoryComponent();

private:
	// Slots
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;
	UPROPERTY()
	TArray<UInventorySlotWidget*> InventorySlots;
	UPROPERTY()
	USelectedItemWidget* SelectedItemWidget;

	FSelectedItem SelectedItem;
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

};
