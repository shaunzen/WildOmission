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

	void AddChatMessage(APlayerState* Sender, const FString& Message, bool bConnectionUpdate = false);
	
	FChatReplicatedSignature OnNewMessage;

	void GetChatMessages(TArray<FChatMessage>& OutChatMessages);

private:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChatMessages)
	TArray<FChatMessage> ChatMessages;
	
	UFUNCTION()
	void OnRep_ChatMessages();

};
