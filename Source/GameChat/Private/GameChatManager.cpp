// Copyright Telephone Studios. All Rights Reserved.


#include "GameChatManager.h"
#include "GameFramework/PlayerState.h"

static AGameChatManager* Instance;

// Sets default values
AGameChatManager::AGameChatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AGameChatManager::BeginPlay()
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

void AGameChatManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Instance = nullptr;
}

void AGameChatManager::SendMessage(APlayerState* SenderPlayerState, const FString& Message, bool ConnectionUpdate)
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

void AGameChatManager::Multi_PushMessageToClients_Implementation(const FChatMessage& ChatMessage)
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

AGameChatManager* AGameChatManager::GetGameChatManager()
{
	return Instance;
}

bool AGameChatManager::IsValidMessage(const FChatMessage& ChatMessage)
{
	if (ChatMessage.Message == TEXT("") || ChatMessage.SenderName == TEXT(""))
	{
		return false;
	}

	return true;
}

TArray<FChatMessage> AGameChatManager::GetChatMessages()
{
	return ChatMessages;
}