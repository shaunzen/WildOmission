// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WildOmissionGameState.generated.h"

struct FChatMessage;

UCLASS()
class WILDOMISSION_API AWildOmissionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SendChatMessage(APlayerState* Sender, const FString& Message);

private:
	TArray<FChatMessage> ChatMessages;
};
