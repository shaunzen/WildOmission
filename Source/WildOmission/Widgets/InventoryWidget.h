// (c) 2023 Telephone Studios, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UWrapBox;
class UBorder;
class UInventorySlotWidget;
class UInventoryComponent;

UCLASS()
class WILDOMISSION_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(UInventoryComponent* InInventoryComponent);
	
	void Open();
	void Close();

	UInventoryComponent* GetInventoryComponent();
private:
	// Slots
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;
	UPROPERTY()
	TArray<UInventorySlotWidget*> InventorySlots;

	// Bind Widget Elements
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InventoryName;
	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;
	UPROPERTY(meta = (BindWidget))
	UWrapBox* ToolbarWrapBox;

	// Inventory to read from
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
};
