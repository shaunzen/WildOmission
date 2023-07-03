// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DurabilityInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDurabilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DURABILITY_API IDurabilityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetCurrentDurability() = 0;
	virtual float GetMaxDurability() = 0;

};
