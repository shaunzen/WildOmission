// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryParentWidget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryParentWidget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInventoryParentWidget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual class UHoveredItemNameTag* GetHoveredItemNameTag() const = 0;
	virtual bool SelectedItemVisible() const = 0;
	virtual bool IsInventoryMenuOpen() const = 0;

};
