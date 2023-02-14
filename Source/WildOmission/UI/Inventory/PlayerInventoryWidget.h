// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "PlayerInventoryWidget.generated.h"

class UPlayerInventoryComponent;

UCLASS()
class WILDOMISSION_API UPlayerInventoryWidget : public UInventoryWidget
{
	GENERATED_BODY()
	
public:
	UPlayerInventoryWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void Setup(UInventoryComponent* InInventoryComponent) override;

	virtual void Refresh() override;

protected:
	UPROPERTY(Meta = (BindWidget))
	UUniformGridPanel* ToolbarGridPanel;

private:
	UPROPERTY()
	UPlayerInventoryComponent* PlayerInventoryComponent;

	void CreateToolbarSlots();

};
