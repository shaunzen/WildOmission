// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockModifier.generated.h"

class ALockDeployable;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULockModifier : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEPLOYABLES_API ILockModifier
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OpenLockMenu(ALockDeployable* Lock) = 0;

};
