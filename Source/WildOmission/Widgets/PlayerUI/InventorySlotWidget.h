// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WildOmission/Structs/InventoryItem.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UBorder;
class UInventoryWidget;

UCLASS()
class WILDOMISSION_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(UInventoryWidget* InOwner, uint8 InColumn, uint8 InRow);
	void SetItem(FInventoryItem Item);
	void ClearItem();

	FInventoryItem* GetCurrentItem();

	bool IsFull() const;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
	UBorder* SlotBorder;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* ItemIconBorder;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuantityText;

	FInventoryItem CurrentItem;

	UPROPERTY()
	UInventoryWidget* Owner;

	UPROPERTY()
	uint8 Column;

	UPROPERTY()
	uint8 Row;
	
	bool bIsFull;
	
	void LeftMouseDrag();
	void LeftMouseDrop();
	void RightMouseDrag();
	void RightMouseDrop();

	struct FItem* GetSelectedItemData();
	
};
