// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/ChatMessage.h"
#include "GameChatHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChatMessageRecievedSignature);

UCLASS()
class GAMECHAT_API AGameChatHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameChatHandler();
	
	UFUNCTION(Server, Reliable)
	void Server_SendMessage(APlayerState* SenderPlayerState, const FString& Message, bool ConnectionUpdate = false);

	static AGameChatHandler* GetInstance();

	static bool IsValidMessage(const FChatMessage& ChatMessage);
	
	static TArray<FChatMessage> GetChatMessages();

	FOnChatMessageRecievedSignature OnMessageRecieved;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(NetMulticast, Reliable)
	void Multi_PushMessageToClients(const FChatMessage& ChatMessage);

};
