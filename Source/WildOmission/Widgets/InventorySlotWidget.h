// (c) 2023 Telephone Studios, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UBorder;
class UInventoryComponent;

UCLASS()
class WILDOMISSION_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void Setup(FName InItemName, int32 InQuantity, UInventoryComponent* InInventoryComponent);
private:

	UPROPERTY(meta = (BindWidget))
	UBorder* SlotImageBorder;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuantityText;
	UPROPERTY()
	FName ItemName;
	UPROPERTY()
	int32 Quantity;
	
	UPROPERTY()
	UInventoryComponent* OwnerInventoryComponent;
};
