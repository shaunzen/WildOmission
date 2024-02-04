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

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = 100;
	SessionSettings.bAllowInvites = true;
	SessionSettings.bIsDedicated = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = false;
	SessionSettings.bUseLobbiesIfAvailable = false;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	
	const FString ServerName = TEXT("Dedicated Server (WIP)");
	const FString LevelFile = TEXT("LV_Procedural");

	SessionSettings.Set(UWildOmissionGameInstance::GetFriendsOnlySettingsKey(), false, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(UWildOmissionGameInstance::GetServerNameSettingsKey(), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(UWildOmissionGameInstance::GetLevelFileSettingsKey(), LevelFile, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(TEXT("MAPNAME"), LevelFile, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(UWildOmissionGameInstance::GetGameVersionSettingsKey(), WildOmissionGameInstace->GetVersion(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, UWildOmissionGameInstance::GetSessionName(), SessionSettings);
}
