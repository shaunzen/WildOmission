// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableWeatherHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USavableWeatherHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAVESYSTEM_API ISavableWeatherHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetNextStormSpawnTime() const = 0;
	virtual void SetNextStormSpawnTime(float NewSpawnTime) = 0;

};
