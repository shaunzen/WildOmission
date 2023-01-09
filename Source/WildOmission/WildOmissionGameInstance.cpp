// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"
#include "MenuSystem/MainMenuWidget.h"

// Static session information
const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UWildOmissionGameInstance::UWildOmissionGameInstance(const FObjectInitializer& ObjectIntializer)
{
	ConstructorHelpers::FClassFinder<UMainMenuWidget> MainMenuBlueprint(TEXT("/Game/Blueprints/MenuSystem/WBP_MainMenu"));
	if (MainMenuBlueprint.Class)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Main Menu widget blueprint"));
		MainMenuWidgetBlueprintClass = MainMenuBlueprint.Class;
	}
}

void UWildOmissionGameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem == nullptr)
	{
		return;
	}
	SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		return;
	}
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UWildOmissionGameInstance::OnCreateSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UWildOmissionGameInstance::OnDestroySessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UWildOmissionGameInstance::OnFindSessionsComplete);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UWildOmissionGameInstance::OnJoinSessionComplete);
	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UWildOmissionGameInstance::OnNetworkFailure);
	}
}

void UWildOmissionGameInstance::ShowMainMenuWidget()
{
	if (MainMenuWidgetBlueprintClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create the main menu widget, blueprint class was nullptr"));
		return;
	}
	MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetBlueprintClass);
	if (MainMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create the main menu widget"))
		return;
	}

	MainMenuWidget->Setup();
}

void UWildOmissionGameInstance::StartSession()
{
	if (SessionInterface.IsValid() == false)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Starting Session"));
	SessionInterface->StartSession(SESSION_NAME);
}

void UWildOmissionGameInstance::RefreshServerList()
{
	UE_LOG(LogTemp, Display, TEXT("Refreshing Server List"));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Session Search"));
		return;
	}
	// Uncomment for lan results using null
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

//****************************
// HOSTING/JOINING
//****************************
void UWildOmissionGameInstance::Host(FString ServerName)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Hosting Server Named: %s"), *ServerName);
	DesiredServerName = ServerName;
	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
	else
	{
		CreateSession();
	}
}

void UWildOmissionGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid() || MainMenuWidget == nullptr)
	{
		return;
	}
	MainMenuWidget->Teardown();

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	UE_LOG(LogTemp, Warning, TEXT("Joining Server Index: %i"), Index);
}

void UWildOmissionGameInstance::CreateSession()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

//****************************
// Subsystem Delegates
//****************************
void UWildOmissionGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (Success == false || MainMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create session"));
		return;
	}

	// Remove the menu from viewport
	MainMenuWidget->Teardown();
	
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	// Server travel to the game level
	World->ServerTravel("/Game/Maps/Level_Sandbox?listen");
}

void UWildOmissionGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success == false)
	{
		return;
	}
	CreateSession();
}

void UWildOmissionGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success == false || SessionSearch.IsValid() == false || MainMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Found sessions but they are invalid"));
		return;
	}
	/*
	Setup the server list
	*/
	TArray<FServerData> ServerNames;
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		FServerData Data;
		Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
		Data.HostUsername = SearchResult.Session.OwningUserName;
		FString ServerName;
		if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
		{
			Data.Name = ServerName;
		}
		else
		{
			Data.Name = "Invalid Name";
		}
		ServerNames.Add(Data);
	}
	MainMenuWidget->SetServerList(ServerNames);
}

void UWildOmissionGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface.IsValid() == false)
	{
		return;
	}
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not resolve connect string"));
		return;
	}
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UWildOmissionGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	//TODO goto main menu
	UE_LOG(LogTemp, Error, TEXT("Network failure disconnecting..."));
	UE_LOG(LogTemp, Error, TEXT("Damnit larch you forgot to add disconnecting"));
}
