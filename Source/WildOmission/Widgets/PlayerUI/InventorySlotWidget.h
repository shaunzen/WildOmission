// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UBorder;
class UInventoryWidget;

USTRUCT()
struct FSlotItem
{
	GENERATED_BODY()

	FName Name;
	int32 Quantity;
	
	void Set(FName InName = FName(""), int32 InQuantity = 0)
	{
		Name = InName;
		Quantity = InQuantity;
	}
	void Clear()
	{
		Name = FName("");
		Quantity = 0;
	}
};

UCLASS()
class WILDOMISSION_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(UInventoryWidget* InOwner);
	void SetItem(FSlotItem Item);
	void ClearItem();

	FSlotItem* GetCurrentItem();

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

	FSlotItem CurrentItem;

	UPROPERTY()
	UInventoryWidget* Owner;

	bool bIsFull;
	
	void LeftMouseDrag();
	void LeftMouseDrop();
	void RightMouseDrag();
	void RightMouseDrop();

	struct FItem* GetSelectedItemData();
	
};
