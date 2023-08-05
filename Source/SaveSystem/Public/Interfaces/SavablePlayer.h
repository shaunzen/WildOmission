// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/PlayerSave.h"
#include "SavablePlayer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USavablePlayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAVESYSTEM_API ISavablePlayer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FPlayerSave SavePlayer() = 0;
	virtual void LoadPlayerSave(const FPlayerSave& Save) = 0;

	virtual FString GetUniqueID() = 0;
	virtual bool IsHost() = 0;

};
