// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/InventoryItem.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UBorder;
class UProgressBar;
class UInventoryWidget;

UCLASS()
class INVENTORY_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInventorySlotWidget(const FObjectInitializer& ObjectInitializer);

	void Setup(UInventoryWidget* InOwner, const int32& InIndex);

	void SetItem(const FInventoryItem& Item);
	void ClearItem();

	UFUNCTION(BlueprintCallable)
	int32 GetIndex() const;
	
	void SetSelected(bool InSelected);

	UFUNCTION(BlueprintCallable)
	bool IsSelected() const;

	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
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

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* DurabilityBar;

	UPROPERTY()
	UInventoryWidget* Owner;

	UPROPERTY()
	int32 Index;
	
	FName CurrentItemName;
	int32 CurrentItemQuantity;

	UPROPERTY()
	bool Selected;

	UPROPERTY()
	bool Hovering;

	void ShowHoveredItemNameTag();
	void ShowAdditionalItemNameTagDetails();
	void HideHoveredItemNameTag();

};
