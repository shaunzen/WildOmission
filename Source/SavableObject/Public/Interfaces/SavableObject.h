// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USavableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAVABLEOBJECT_API ISavableObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadComplete();

	UFUNCTION()
	virtual FName GetIdentifier() const;

};
