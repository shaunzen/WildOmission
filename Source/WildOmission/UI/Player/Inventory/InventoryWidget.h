// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
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
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

	// Setup
	void SetSelectedItemWidget(USelectedItemWidget* InWidget);
	void Setup(UInventoryComponent* InInventoryComponent);

	void Open();
	void Close();

	void Refresh();
	
	UInventoryComponent* GetInventoryComponent();

private:
	// Slots
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	UPROPERTY()
	TArray<UInventorySlotWidget*> Slots;
	
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

	void CreateSlots(bool CreateToolbar = false);

	void CreateToolbarSlots();
	void CreateInventorySlots();

};
