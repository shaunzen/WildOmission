// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerControllerMessageSender.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerControllerMessageSender : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMECHAT_API IPlayerControllerMessageSender
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SendMessage(APlayerState* Sender, const FString& Message) = 0;

};
