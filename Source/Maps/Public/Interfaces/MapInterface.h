// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MapInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMapInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAPS_API IMapInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void SetSavedLocationToCurrentLocation() = 0;

	UFUNCTION()
	virtual void SetLastDeathLocation(const FVector& DeathLocation) = 0;

	UFUNCTION()
	virtual FVector GetCurrentCoordinateLocation() = 0;

	UFUNCTION()
	virtual FVector GetSavedLocation() = 0;
	
	UFUNCTION()
	virtual FVector GetLastDeathLocation() = 0;

};
