// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameSession.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "WildOmissionGameInstance.h"

void AWildOmissionGameSession::RegisterServer()
{
	Super::RegisterServer();

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	UWildOmissionGameInstance* WildOmissionGameInstace = UWildOmissionGameInstance::GetWildOmissionGameInstance(World);
	if (WildOmissionGameInstace == nullptr || !WildOmissionGameInstace->IsDedicatedServerInstance())
	{
		return;
	}
	
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem == nullptr)
	{
		return;
	}

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FOnlineSessionSettings Settings;
	Settings.NumPublicConnections = 3;
	Settings.bShouldAdvertise = true;
	Settings.bAllowJoinInProgress = true;
	Settings.bIsLANMatch = true;
	Settings.bUsesPresence = true;
	Settings.bAllowJoinViaPresence = true;

	SessionInterface->CreateSession(0, GameSessionName, Settings);
	return;
}
