// Copyright Telephone Studios. All Rights Reserved.


#include "GameChatHandler.h"
#include "Structs/ChatMessage.h"
#include "GameFramework/PlayerState.h"

static AGameChatHandler* Instance;
static TArray<FChatMessage> ChatMessages;

// Sets default values
AGameChatHandler::AGameChatHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AGameChatHandler::SendMessage(APlayerState* SenderPlayerState, const FString& Message, bool ConnectionUpdate)
{
	Server_SendMessage(SenderPlayerState, Message, ConnectionUpdate);
}

void AGameChatHandler::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->IsEditorWorld() && IsValid(Instance))
	{
		return;
	}

	Instance = this;
}

void AGameChatHandler::Server_SendMessage_Implementation(APlayerState* SenderPlayerState, const FString& Message, bool ConnectionUpdate)
{
	if (SenderPlayerState == nullptr || Message.IsEmpty())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Server_SendMessage Was Called."));

	FChatMessage ChatMessage;
	ChatMessage.SenderName = SenderPlayerState->GetPlayerName();
	ChatMessage.Message = Message;
	ChatMessage.ConnectionUpdate = ConnectionUpdate;

	Multi_PushMessageToClients(ChatMessage);
}

void AGameChatHandler::Multi_PushMessageToClients_Implementation(const FChatMessage& ChatMessage)
{
	UWorld* World = GetWorld();
	if (!IsValidMessage(ChatMessage) || World == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Client Recieved Message."));

	FChatMessage NewMessage = ChatMessage;
	NewMessage.TimeRecieved = World->GetRealTimeSeconds();

	ChatMessages.Insert(NewMessage, 0);

	if (ChatMessages.Num() > 30)
	{
		ChatMessages.RemoveAt(ChatMessages.Num() - 1);
	}

	if (OnMessageRecieved.IsBound())
	{
		OnMessageRecieved.Broadcast();
	}
}

AGameChatHandler* AGameChatHandler::GetInstance()
{
	return Instance;
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