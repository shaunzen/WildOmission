// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UBorder;
class UInventoryWidget;

UCLASS()
class WILDOMISSION_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(UInventoryWidget* InOwner, bool bInToolbarSlot = false);
	void SetItem(FName ItemName, int32 ItemQuantity);

	FName GetCurrentItemName();
	int32 GetCurrentItemQuantity();
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

	UPROPERTY()
	FName CurrentItemName;
	UPROPERTY()
	int32 CurrentItemQuantity;

	UPROPERTY()
	UInventoryWidget* Owner;

	UPROPERTY()
	bool bIsFull;

	UPROPERTY()
	bool bToolbarSlot;

	UFUNCTION()
		void OnPressed();

};
