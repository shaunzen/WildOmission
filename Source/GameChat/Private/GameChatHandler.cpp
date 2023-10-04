// Copyright Telephone Studios. All Rights Reserved.


#include "GameChatHandler.h"
#include "Structs/ChatMessage.h"

static TArray<FChatMessage> ChatMessages;

// Sets default values
AGameChatHandler::AGameChatHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AGameChatHandler::Server_SendMessage_Implementation(const FChatMessage& ChatMessage)
{
	if (!IsValidMessage(ChatMessage))
	{
		return;
	}

	Multi_PushMessageToClients(ChatMessage);
}

void AGameChatHandler::Multi_PushMessageToClients_Implementation(const FChatMessage& ChatMessage)
{
	UWorld* World = GetWorld();
	if (!IsValidMessage(ChatMessage) || World == nullptr)
	{
		return;
	}

	FChatMessage NewMessage = ChatMessage;
	NewMessage.TimeRecieved = World->GetRealTimeSeconds();

	ChatMessages.Insert(NewMessage, 0);

	if (ChatMessages.Num() > 30)
	{
		ChatMessages.RemoveAt(ChatMessages.Num() - 1);
	}
}

bool AGameChatHandler::IsValidMessage(const FChatMessage& ChatMessage)
{
	if (ChatMessage.Message == TEXT("") || ChatMessage.SenderName == TEXT(""))
	{
		return false;
	}

	return true;
}

TArray<FChatMessage> AGameChatHandler::GetChatMessages()
{
	return ChatMessages;
}