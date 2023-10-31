// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MENUSYSTEM_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void StartSingleplayer(const FString& WorldName) = 0;

	UFUNCTION()
	virtual void Host(const FString& ServerName, const FString& WorldName, bool FriendsOnly = false) = 0;
	
	UFUNCTION()
	virtual void Join(const uint32& Index) = 0;

	UFUNCTION()
	virtual void RefreshServerList() = 0;

	UFUNCTION()
	virtual void QuitToMenu() = 0;

	static TArray<FString> GetAllWorldNames();
	static bool WorldAlreadyExists(const FString& WorldNameToTest);

};
