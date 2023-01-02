// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UBorder;
class UButton;
class UInventoryWidget;

UCLASS()
class WILDOMISSION_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(UInventoryWidget* InOwner, bool bInToolbarSlot = false);
	void SetItem(FName ItemName, int32 ItemQuantity);
	
	// TODO getters for item and quantity
	FName GetCurrentItemName();
	int32 GetCurrentItemQuantity();
private:

	UPROPERTY(meta = (BindWidget))
	UButton* SlotButton;
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
		bool bToolbarSlot;

	UFUNCTION()
		void OnPressed();
};
