// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WildOmission/Core/Structs/ChatMessage.h"
#include "WildOmissionGameState.generated.h"

UCLASS()
class WILDOMISSION_API AWildOmissionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SendChatMessage(APlayerState* Sender, const FString& Message);

private:
	UPROPERTY(Replicated)
	TArray<FChatMessage> ChatMessages;

	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(APlayerState* Sender, const FString& Message);
};
