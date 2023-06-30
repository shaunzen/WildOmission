// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

/**
 * this will be where the open inventories, selected item, and hover item ids will be handled
 * this will free up lots of code from the hud class and make the inventory a more self contained module
 */
UCLASS()
class INVENTORY_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
};
