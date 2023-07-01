// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemContainer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemContainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IItemContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnOccupy() = 0;

};
