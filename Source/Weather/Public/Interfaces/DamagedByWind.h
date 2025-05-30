// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamagedByWind.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamagedByWind : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WEATHER_API IDamagedByWind
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void ApplyWindDamage(AActor* WindCauser, float DamageMultiplier = 1.0f) = 0;

	float GetBaseWindDamage() const;

protected:

	// Applied continuously, don't put too high.
	float BaseWindDamage = 10.0f;

};
