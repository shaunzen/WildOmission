// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterSettingsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterSettingsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMESETTINGS_API ICharacterSettingsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyInputSettings() = 0;
	virtual void ApplyFieldOfView() = 0;
	virtual void ApplyPostProcessing() = 0;
};
