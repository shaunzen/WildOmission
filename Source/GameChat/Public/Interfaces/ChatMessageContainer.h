// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/ChatMessage.h"
#include "ChatMessageContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChatReplicatedSignature);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UChatMessageContainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMECHAT_API IChatMessageContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AddChatMessage(APlayerState* Sender, const FString& Message, bool bConnectionUpdate = false) = 0;
	virtual void GetChatMessages(TArray<FChatMessage>& OutChatMessages) = 0;
	FChatReplicatedSignature OnChatReplicated;

};
