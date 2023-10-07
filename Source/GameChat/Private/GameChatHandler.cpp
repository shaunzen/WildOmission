// Copyright Telephone Studios. All Rights Reserved.


#include "GameChatHandler.h"
#include "GameFramework/PlayerState.h"

static AGameChatHandler* Instance;

// Sets default values
AGameChatHandler::AGameChatHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AGameChatHandler::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (World == nullptr || World->IsEditorWorld() && IsValid(Instance))
	{
		return;
	}

	Instance = this;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	SetOwner(PlayerController);
}

void AGameChatHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Instance = nullptr;
}

void AGameChatHandler::SendMessage(APlayerState* SenderPlayerState, const FString& Message, bool ConnectionUpdate)
{
	if (SenderPlayerState == nullptr || Message.IsEmpty())
	{
		return;
	}

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

AGameChatHandler* AGameChatHandler::GetGameChatHandler()
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