// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableTimeOfDayHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USavableTimeOfDayHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAVESYSTEM_API ISavableTimeOfDayHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetDaysPlayed(int32 InDaysPlayed) = 0;
	virtual int32 GetDaysPlayed() const = 0;

	virtual void SetNormalizedProgressThroughDay(float InProgress) = 0;
	virtual float GetNormalizedProgressThroughDay() const = 0;

};
