// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.h"
#include "WildOmission/Structs/InventoryItem.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UUniformGridPanel;
class UBorder;
class USelectedItemWidget;
class UInventoryComponent;

UCLASS()
class WILDOMISSION_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Setup
	void SetSelectedItemWidget(USelectedItemWidget* InWidget);
	void SetComponent(UInventoryComponent* InInventoryComponent);

	void Open();
	void Close();
	
	UInventoryComponent* GetInventoryComponent();

private:
	// Slots
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY()
	TArray<UInventorySlotWidget*> InventorySlots;
	
	UPROPERTY()
	USelectedItemWidget* SelectedItemWidget;

	// Bind Widget Elements
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InventoryName;
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* InventoryGridPanel;
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* ToolbarGridPanel;
	
	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	void CreateSlots();

	void CreateToolbarSlots();
	void CreateInventorySlots();

};
