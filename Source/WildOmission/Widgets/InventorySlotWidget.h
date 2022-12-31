// (c) 2023 Telephone Studios, all rights reserved.

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
	// TODO toolbar?
	void Setup(UInventoryWidget* InOwner);
	void SetItem(FName ItemName, int32 ItemQuantity);
	
	// TODO getters for item and quantity

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
};
