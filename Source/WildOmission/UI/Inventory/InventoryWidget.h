// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UUniformGridPanel;
class UBorder;
class UInventoryComponent;

UCLASS()
class WILDOMISSION_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void Setup(UInventoryComponent* InInventoryComponent);
	virtual void Refresh();
	virtual void RefreshSlot(const int32& SlotIndex);

	void CreateSlots();

	void Open();
	void Close();

	UInventoryComponent* GetInventoryComponent();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InventoryName;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* InventoryGridPanel;
	
	// Slots
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	UPROPERTY()
	TArray<UInventorySlotWidget*> Slots;

private:
	UPROPERTY()
	UInventoryComponent* InventoryComponent;

};
