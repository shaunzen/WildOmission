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
	if (!OnChatReplicated.IsBound())
	{
		return;
	}

	OnChatReplicated.Broadcast();
}

void AWildOmissionGameState::AddChatMessage(APlayerState* Sender, const FString& Message, bool bConnectionUpdate)
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
	NewMessage.ConnectionUpdate = bConnectionUpdate;

	ChatMessages.Insert(NewMessage, 0);

	if (ChatMessages.Num() > 30)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cleaning old messages."));
		ChatMessages.RemoveAt(ChatMessages.Num() - 1);
	}

	OnRep_ChatMessages();
}

void AWildOmissionGameState::GetChatMessages(TArray<FChatMessage>& OutChatMessages)
{
	OutChatMessages = ChatMessages;
}