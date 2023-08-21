// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterSettingsUser.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterSettingsUser : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMESETTINGS_API ICharacterSettingsUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyFieldOfView() = 0;

};
