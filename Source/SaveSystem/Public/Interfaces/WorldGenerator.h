// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WildOmissionSaveGame.h"
#include "WorldGenerator.generated.h"

class ASaveHandler;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWorldGenerator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAVESYSTEM_API IWorldGenerator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GenerateLevel(ASaveHandler* InstigatingSaveHandler, UWildOmissionSaveGame* InSaveFile) = 0;
	
};
