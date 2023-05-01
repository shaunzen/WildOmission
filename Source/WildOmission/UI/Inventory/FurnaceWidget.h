// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "FurnaceWidget.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API UFurnaceWidget : public UInventoryWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* ToggleButton;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ToggleText;

};
