// Copyright Telephone Studios. All Rights Reserved.

#include "WildOmissionGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "Widgets/MainMenuWidget.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UWildOmissionGameInstance::UWildOmissionGameInstance(const FObjectInitializer& ObjectInitializer)
{
	// Get the menu widget
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/Blueprints/Widgets/WBP_MainMenu"));
	if (MainMenuBPClass.Class == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find the main menu widget blueprint."));
		return;
	}
	MainMenuWidgetClass = MainMenuBPClass.Class;
}

void UWildOmissionGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem == nullptr)
	{
		return;
	}
	if (GEngine == nullptr)
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

	GEngine->OnNetworkFailure().AddUObject(this, &UWildOmissionGameInstance::OnNetworkFailure);
}

void UWildOmissionGameInstance::LoadMainMenuWidget()
{
	if (MainMenuWidgetClass == nullptr)
	{
		return;
	}

	MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
	if (MainMenuWidget == nullptr)
	{
		return;
	}

	MainMenuWidget->Setup();
	MainMenuWidget->SetMainMenuInterface(this);
}

void UWildOmissionGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (!SessionInterface.IsValid())
	{
		return;
	}
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

void UWildOmissionGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success == false)
	{
		return;
	}
	CreateSession();
}

void UWildOmissionGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	// TODO Redirect to the main menu
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

void UWildOmissionGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	UEngine* Engine = GetEngine();
	UWorld* World = GetWorld();
	if (Success == false || Engine == nullptr || World == nullptr)
	{
		return;
	}
	// TODO remove the menu widget
	MainMenuWidget->TearDown();
	Engine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Hosting"));
	World->ServerTravel("/Game/Maps/Level_Sandbox?listen");
}

void UWildOmissionGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (!SessionSearch.IsValid())
	{
		return;
	}
	//SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	UE_LOG(LogTemp, Display, TEXT("GameInstance Session Search began"));
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UWildOmissionGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success == false || !SessionSearch.IsValid() || !MainMenuWidget)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("GameInstance Session Search completed"));
	TArray<FServerData> ServerNames;
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		UE_LOG(LogTemp, Display, TEXT("GameInstance found session: %s"), *SearchResult.GetSessionIdStr());
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
			Data.Name = "Name Failure";
		}
		ServerNames.Add(Data);
	}
	MainMenuWidget->SetServerList(ServerNames);
}

void UWildOmissionGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
	{
		return;
	}
	// TODO remove menu widget
	MainMenuWidget->TearDown();
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UWildOmissionGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString Address;
	UEngine* Engine = GetEngine();
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!SessionInterface.IsValid() 
		|| !SessionInterface->GetResolvedConnectString(SessionName, Address) 
		|| Engine == nullptr
		|| PlayerController == nullptr)
	{
		return;
	}
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UWildOmissionGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UWildOmissionGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	PlayerController->ClientTravel("/Game/Maps/Level_MainMenu", ETravelType::TRAVEL_Absolute);
}