// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameState.h"
#include "WildOmission/Core/Structs/ChatMessage.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void AWildOmissionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWildOmissionGameState, ChatMessages);
}

void AWildOmissionGameState::SendChatMessage(APlayerState* Sender, const FString& Message)
{
	FString SenderName = Sender->GetPlayerName();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%s is sending %s"), *SenderName, *Message));
}