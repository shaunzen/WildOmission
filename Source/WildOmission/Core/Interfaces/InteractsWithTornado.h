// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractsWithTornado.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractsWithTornado : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WILDOMISSION_API IInteractsWithTornado
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void OnContactWithTornado() = 0;
};
