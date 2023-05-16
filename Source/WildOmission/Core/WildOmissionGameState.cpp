// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameState.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void AWildOmissionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWildOmissionGameState, ChatMessages);
}

void AWildOmissionGameState::OnRep_ChatMessages()
{
	if (!OnNewMessage.IsBound())
	{
		return;
	}

	OnNewMessage.Broadcast();
}

void AWildOmissionGameState::SendChatMessage(APlayerState* Sender, const FString& Message)
{
	Server_SendChatMessage(Sender, Message);
}

void AWildOmissionGameState::Server_SendChatMessage_Implementation(APlayerState* Sender, const FString& Message)
{
	if (Sender == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to proccess message, Unknown Sender."));
		return;
	}

	FChatMessage NewMessage;
	NewMessage.SenderName = Sender->GetPlayerName();
	NewMessage.Message = Message;
	NewMessage.TimeSent = GetWorld()->GetRealTimeSeconds();

	ChatMessages.Push(NewMessage);
}