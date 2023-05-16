// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WildOmission/Core/Structs/ChatMessage.h"
#include "WildOmissionGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChatReplicatedSignature);

UCLASS()
class WILDOMISSION_API AWildOmissionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SendChatMessage(APlayerState* Sender, const FString& Message);
	
	FChatReplicatedSignature OnNewMessage;

private:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChatMessages)
	TArray<FChatMessage> ChatMessages;
	
	UFUNCTION()
	void OnRep_ChatMessages();

	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(APlayerState* Sender, const FString& Message);
};
