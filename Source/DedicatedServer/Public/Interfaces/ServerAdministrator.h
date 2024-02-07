// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ServerAdministrator.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UServerAdministrator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEDICATEDSERVER_API IServerAdministrator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetAdministrator(bool InAdministrator) = 0;
	virtual bool IsAdministrator() const = 0;

	virtual void KickPlayer(APlayerController* PlayerControllerToKick) = 0;

};
