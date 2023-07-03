// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameSaveLoadController.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameSaveLoadController : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAVESYSTEM_API IGameSaveLoadController
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartLoading() = 0;
	virtual void StopLoading() = 0;
	virtual void SetLoadingTitle(const FString& InLoadingTitle) = 0;
	virtual void SetLoadingSubtitle(const FString& InLoadingSubtitle) = 0;
	virtual void CreateWorld(const FString& WorldName) = 0;
};
