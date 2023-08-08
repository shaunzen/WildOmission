// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Interfaces/ChatMessageContainer.h"
#include "WildOmissionGameState.generated.h"

UCLASS()
class WILDOMISSIONCORE_API AWildOmissionGameState : public AGameStateBase, public IChatMessageContainer
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Begin IChatMessageContainer Implementation
	virtual void AddChatMessage(APlayerState* Sender, const FString& Message, bool bConnectionUpdate = false) override;
	virtual void GetChatMessages(TArray<FChatMessage>& OutChatMessages) override;
	// End IChatMessageContainer Implementation

	UFUNCTION(BlueprintCallable)
	bool IsCheatingEnabled() const;

private:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChatMessages)
	TArray<FChatMessage> ChatMessages;
	
	UFUNCTION()
	void OnRep_ChatMessages();

};
