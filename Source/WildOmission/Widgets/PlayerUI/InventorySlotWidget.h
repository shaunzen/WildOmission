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
	void Setup(UInventoryWidget* InOwner, const int32& InIndex);
	void SetItem(const FInventoryItem& Item);
	void ClearItem();

	FInventoryItem* GetCurrentItem();

	int32 GetIndex();

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
	int32 Index;
	
};
