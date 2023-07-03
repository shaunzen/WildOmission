// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ContainerOpener.generated.h"

class AItemContainerBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UContainerOpener : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEPLOYABLES_API IContainerOpener
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InvokeOpenContainer(AItemContainerBase* Container) = 0;

};
