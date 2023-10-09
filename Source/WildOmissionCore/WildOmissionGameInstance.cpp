// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineFriendsInterface.h" 
#include "UI/MainMenuWidget.h"
#include "UI/GameplayMenuWidget.h"
#include "UI/LoadingMenuWidget.h"
#include "WildOmissionSaveGame.h"
#include "WildOmissionGameUserSettings.h"
#include "GameFramework/PlayerState.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"

// Static session information
const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");
const static FName FRIENDS_ONLY_SETTINGS_KEY = TEXT("FriendsOnlySession");
const static FName LEVEL_FILE_SETTINGS_KEY = TEXT("LevelFile");
const static FName SEARCH_PRESENCE = TEXT("PRESENCESEARCH");
const static FString GameVersion = TEXT("Pre Alpha 0.9.9_02");

static USoundMix* MasterSoundMixModifier = nullptr;
static USoundClass* MasterSoundClass = nullptr;
static USoundClass* MusicSoundClass = nullptr;
static USoundClass* DeployablesSoundClass = nullptr;
static USoundClass* EnvironmentSoundClass = nullptr;
static USoundClass* FriendlyCreaturesSoundClass = nullptr;
static USoundClass* HostileCreaturesSoundClass = nullptr;
static USoundClass* PlayersSoundClass = nullptr;
static USoundClass* WeatherSoundClass = nullptr;

UWildOmissionGameInstance::UWildOmissionGameInstance(const FObjectInitializer& ObjectIntializer)
{
	LoadingMenuWidget = nullptr;
	MainMenuWidget = nullptr;
	GameplayMenuWidget = nullptr;
	Loading = false;
	FriendsOnlySession = false;
	OnMainMenu = false;
	
	static ConstructorHelpers::FClassFinder<UMainMenuWidget> MainMenuBlueprint(TEXT("/Game/MenuSystem/UI/WBP_MainMenu"));
	if (MainMenuBlueprint.Succeeded())
	{
		MainMenuWidgetBlueprintClass = MainMenuBlueprint.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UGameplayMenuWidget> GameplayMenuBlueprint(TEXT("/Game/MenuSystem/UI/WBP_GameplayMenu"));
	if (GameplayMenuBlueprint.Succeeded())
	{
		GameplayMenuWidgetBlueprintClass = GameplayMenuBlueprint.Class;
	}
	
	static ConstructorHelpers::FClassFinder<ULoadingMenuWidget> LoadingMenuBlueprint(TEXT("/Game/MenuSystem/UI/WBP_LoadingMenu"));
	if (LoadingMenuBlueprint.Succeeded())
	{
		LoadingMenuWidgetBlueprintClass = LoadingMenuBlueprint.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundMix> MasterSoundMixModifierBlueprint(TEXT("/Game/WildOmissionCore/Audio/MasterSoundClassMix"));
	if (MasterSoundMixModifierBlueprint.Succeeded())
	{
		MasterSoundMixModifier = MasterSoundMixModifierBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> MasterSoundClassBlueprint(TEXT("/Engine/EngineSounds/Master"));
	if (MasterSoundClassBlueprint.Succeeded())
	{
		MasterSoundClass = MasterSoundClassBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> MusicSoundClassBlueprint(TEXT("/Game/WildOmissionCore/Audio/Music"));
	if (MusicSoundClassBlueprint.Succeeded())
	{
		MusicSoundClass = MusicSoundClassBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> DeployablesSoundClassBlueprint(TEXT("/Game/WildOmissionCore/Audio/Deployables"));
	if (DeployablesSoundClassBlueprint.Succeeded())
	{
		DeployablesSoundClass = DeployablesSoundClassBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> EnvironmentSoundClassBlueprint(TEXT("/Game/WildOmissionCore/Audio/Environment"));
	if (EnvironmentSoundClassBlueprint.Succeeded())
	{
		EnvironmentSoundClass = EnvironmentSoundClassBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> FriendlyCreaturesSoundClassBlueprint(TEXT("/Game/WildOmissionCore/Audio/FriendlyCreatures"));
	if (FriendlyCreaturesSoundClassBlueprint.Succeeded())
	{
		FriendlyCreaturesSoundClass = FriendlyCreaturesSoundClassBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> HostileCreaturesSoundClassBlueprint(TEXT("/Game/WildOmissionCore/Audio/HostileCreatures"));
	if (HostileCreaturesSoundClassBlueprint.Succeeded())
	{
		HostileCreaturesSoundClass = HostileCreaturesSoundClassBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> PlayersSoundClassBlueprint(TEXT("/Game/WildOmissionCore/Audio/Players"));
	if (PlayersSoundClassBlueprint.Succeeded())
	{
		PlayersSoundClass = PlayersSoundClassBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> WeatherSoundClassBlueprint(TEXT("/Game/WildOmissionCore/Audio/Weather"));
	if (WeatherSoundClassBlueprint.Succeeded())
	{
		WeatherSoundClass = WeatherSoundClassBlueprint.Object;
	}
}

UWildOmissionGameInstance* UWildOmissionGameInstance::GetWildOmissionGameInstance(UWorld* WorldContextObject)
{
	return Cast<UWildOmissionGameInstance>(WorldContextObject->GetGameInstance());
}

void UWildOmissionGameInstance::Init()
{
	Super::Init();

	ApplyAudioSettings();

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UWildOmissionGameInstance::LoadedNewMap);

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	
	if (Subsystem == nullptr)
	{
		return;
	}

	SessionInterface = Subsystem->GetSessionInterface();
	FriendsInterface = Subsystem->GetFriendsInterface();

	if (!SessionInterface.IsValid() || GEngine == 0)
	{
		return;
	}

	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UWildOmissionGameInstance::OnCreateSessionComplete);
	//SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UWildOmissionGameInstance::OnDestroySessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UWildOmissionGameInstance::OnFindSessionsComplete);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UWildOmissionGameInstance::OnJoinSessionComplete);
	GEngine->OnNetworkFailure().AddUObject(this, &UWildOmissionGameInstance::OnNetworkFailure);

	RunAutoConfigQualitySettings();
}

void UWildOmissionGameInstance::ShowMainMenuWidget()
{
	if (MainMenuWidgetBlueprintClass == nullptr)
	{
		UE_LOG(LogPlayerController, Error, TEXT("Failed to create the main menu widget, blueprint class was nullptr"));
		return;
	}
	
	MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetBlueprintClass);
	if (MainMenuWidget == nullptr)
	{
		UE_LOG(LogPlayerController, Error, TEXT("Failed to create the main menu widget"))
		return;
	}

	MainMenuWidget->Setup(this);
	OnMainMenu = true;
}

void UWildOmissionGameInstance::ShowGameplayMenuWidget()
{
	if (GameplayMenuWidgetBlueprintClass == nullptr)
	{
		UE_LOG(LogPlayerController, Error, TEXT("Failed to create the gameplay menu widget, blueprint class was nullptr"));
		return;
	}
	
	GameplayMenuWidget = CreateWidget<UGameplayMenuWidget>(this, GameplayMenuWidgetBlueprintClass);
	
	if (GameplayMenuWidget == nullptr)
	{
		UE_LOG(LogPlayerController, Error, TEXT("Failed to create gameplay menu widget"));
		return;
	}

	GameplayMenuWidget->Show();
	GameplayMenuWidget->SetMenuInterface(this);
	GameplayMenuWidget->OnClosed.AddDynamic(this, &UWildOmissionGameInstance::ClearGameplayMenuWidget);
}

void UWildOmissionGameInstance::ClearGameplayMenuWidget()
{
	GameplayMenuWidget = nullptr;
}

void UWildOmissionGameInstance::RunAutoConfigQualitySettings(bool Override)
{
	UWildOmissionGameUserSettings* WOUserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (WOUserSettings == nullptr || (WOUserSettings->GetHasRunAutoConfig() && !Override))
	{
		return;
	}

	WOUserSettings->RunHardwareBenchmark(10, 1.0f, 0.2f);
	WOUserSettings->ApplyHardwareBenchmarkResults();

	WOUserSettings->SetHasRunAutoConfig(true);
	WOUserSettings->ApplySettings(false);
}

void UWildOmissionGameInstance::StartLoading()
{
	Loading = true;

	if (LoadingMenuWidgetBlueprintClass == nullptr)
	{
		UE_LOG(LogLoad, Warning, TEXT("Something went wrong displaying the loading screen."));
		return;
	}

	if (LoadingMenuWidget)
	{
		LoadingMenuWidget->Teardown();
		LoadingMenuWidget = nullptr;
	}

	LoadingMenuWidget = CreateWidget<ULoadingMenuWidget>(this, LoadingMenuWidgetBlueprintClass);
	LoadingMenuWidget->Setup();
	LoadingMenuWidget->SetLoadingTitle(LoadingTitle);
	LoadingMenuWidget->SetLoadingStep(LoadingSubtitle);
}

void UWildOmissionGameInstance::StopLoading()
{
	Loading = false;
	if (LoadingMenuWidget == nullptr)
	{
		return;
	}

	LoadingMenuWidget->Teardown();
	LoadingMenuWidget = nullptr;
}

void UWildOmissionGameInstance::SetLoadingTitle(const FString& InLoadingTitle)
{
	LoadingTitle = InLoadingTitle;
	if (LoadingMenuWidget)
	{
		LoadingMenuWidget->SetLoadingTitle(LoadingTitle);
	}
}

void UWildOmissionGameInstance::SetLoadingSubtitle(const FString& InLoadingSubtitle)
{
	LoadingSubtitle = InLoadingSubtitle;
	if (LoadingMenuWidget)
	{
		LoadingMenuWidget->SetLoadingStep(InLoadingSubtitle);
	}
}

void UWildOmissionGameInstance::CreateWorld(const FString& WorldName)
{
	UWildOmissionSaveGame* NewSaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));

	FDateTime Time;
	Time = FDateTime::Now();

	NewSaveGame->LastPlayedTime = Time;

	NewSaveGame->LevelFile = TEXT("LV_Island");
	NewSaveGame->Version = CURRENT_SAVE_FILE_VERSION;

	NewSaveGame->CreationInformation.Name = WorldName;
	NewSaveGame->CreationInformation.Day = Time.GetDay();
	NewSaveGame->CreationInformation.Month = Time.GetMonth();
	NewSaveGame->CreationInformation.Year = Time.GetYear();

	UGameplayStatics::SaveGameToSlot(NewSaveGame, WorldName, 0);
}

void UWildOmissionGameInstance::StartSession()
{
	if (SessionInterface.IsValid() == false)
	{
		UE_LOG(LogOnlineSession, Warning, TEXT("Couldn't start Session, SessionInterface not valid."));
		return;
	}

	UE_LOG(LogOnlineSession, Display, TEXT("Starting Session"));
	SessionInterface->StartSession(SESSION_NAME);
}

void UWildOmissionGameInstance::QuitToMenu()
{
	UE_LOG(LogPlayerController, Display, TEXT("Returning to main menu."));
	ReturnToMainMenu();

	EndExistingSession();
}

void UWildOmissionGameInstance::RefreshServerList()
{
	UE_LOG(LogOnlineSession, Display, TEXT("Refreshing server list."));
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid() == false)
	{
		UE_LOG(LogOnlineSession, Warning, TEXT("Invalid Session Search"));
		return;
	}
	// Uncomment for lan results using null
	//SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 100;
	
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UWildOmissionGameInstance::ApplyAudioSettings()
{
	UWildOmissionGameUserSettings* WOUserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMixModifier, MasterSoundClass, WOUserSettings->GetMasterVolume(), 1.0f, 0.2f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMixModifier, MusicSoundClass, WOUserSettings->GetMusicVolume(), 1.0f, 0.2f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMixModifier, DeployablesSoundClass, WOUserSettings->GetDeployablesVolume(), 1.0f, 0.2f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMixModifier, EnvironmentSoundClass, WOUserSettings->GetEnvironmentVolume(), 1.0f, 0.2f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMixModifier, FriendlyCreaturesSoundClass, WOUserSettings->GetFriendlyCreaturesVolume(), 1.0f, 0.2f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMixModifier, HostileCreaturesSoundClass, WOUserSettings->GetHostileCreaturesVolume(), 1.0f, 0.2f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMixModifier, PlayersSoundClass, WOUserSettings->GetPlayersVolume(), 1.0f, 0.2f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMixModifier, WeatherSoundClass, WOUserSettings->GetWeatherVolume(), 1.0f, 0.2f);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMixModifier);
}

//****************************
// HOSTING/JOINING
//****************************
void UWildOmissionGameInstance::StartSingleplayer(const FString& WorldName)
{
	if (MainMenuWidget == nullptr)
	{
		UE_LOG(LogPlayerController, Error, TEXT("Cannot start singleplayer, MainMenuWidget was nullptr."));
		return;
	}

	OnMainMenu = false;

	// Remove the menu from viewport
	MainMenuWidget->Teardown();

	// Show the loading menu
	StartLoading();
	SetLoadingTitle(TEXT("Loading Game"));
	SetLoadingSubtitle(TEXT("Loading level."));

	WorldToLoad = WorldName;

	UWorld* World = GetWorld();
	UWildOmissionSaveGame* SaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(WorldToLoad, 0));
	if (World == nullptr || SaveGame == nullptr)
	{
		return;
	}

	FString LevelFileString = SaveGame->LevelFile;
	FString LoadString = FString::Printf(TEXT("/Game/WildOmissionCore/Levels/%s?savegame=%s"), *LevelFileString, *WorldName);
	// Server travel to the game level
	World->ServerTravel(LoadString);
}

void UWildOmissionGameInstance::Host(const FString& ServerName, const FString& WorldName, bool FriendsOnly)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	
	DesiredServerName = ServerName;
	WorldToLoad = WorldName;
	FriendsOnlySession = FriendsOnly;
	
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession != nullptr)
	{
		FOnDestroySessionCompleteDelegate DestroySessionDelegate;
		DestroySessionDelegate.BindUObject(this, &UWildOmissionGameInstance::CreateSession);
		SessionInterface->DestroySession(SESSION_NAME, DestroySessionDelegate);
		return;
	}

	CreateSession();
}

void UWildOmissionGameInstance::Join(const uint32& Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid() || MainMenuWidget == nullptr)
	{
		return;
	}
	MainMenuWidget->Teardown();

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	UE_LOG(LogOnlineSession, Display, TEXT("Joining Server Index: %i"), Index);
}

void UWildOmissionGameInstance::CreateSession(FName SessionName, bool Success)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;

	SessionSettings.Set(FRIENDS_ONLY_SETTINGS_KEY, FriendsOnlySession, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(LEVEL_FILE_SETTINGS_KEY, WorldToLoad, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UWildOmissionGameInstance::LoadedNewMap(UWorld* InWorld)
{
	if (Loading == false)
	{
		return;
	}

	StartLoading();
}

void UWildOmissionGameInstance::EndExistingSession()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession == nullptr)
	{
		return;
	}
	
	SessionInterface->DestroySession(SESSION_NAME);
}

//****************************
// Subsystem Delegates
//****************************
void UWildOmissionGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (Success == false || MainMenuWidget == nullptr)
	{
		UE_LOG(LogOnlineSession, Error, TEXT("Could not create session"));
		return;
	}

	OnMainMenu = false;

	// Remove the menu from viewport
	MainMenuWidget->Teardown();
	
	// Show the loading menu
	StartLoading();
	SetLoadingTitle(TEXT("Loading Game"));
	SetLoadingSubtitle(TEXT("Loading level."));

	UWorld* World = GetWorld();
	UWildOmissionSaveGame* SaveGame = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(WorldToLoad, 0));
	if (World == nullptr || SaveGame == nullptr)
	{
		return;
	}
	FString FriendsOnlyString = FString::Printf(TEXT("%i"), FriendsOnlySession);
	FString LevelFileString = SaveGame->LevelFile;
	FString LoadString = FString::Printf(TEXT("/Game/WildOmissionCore/Levels/%s?listen?savegame=%s?friendsonly="), *LevelFileString, *WorldToLoad, *FriendsOnlyString);
	// Server travel to the game level
	World->ServerTravel(LoadString);
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
	if (OnMainMenu == false || Success == false || SessionSearch.IsValid() == false || MainMenuWidget == nullptr)
	{
		UE_LOG(LogOnlineSession, Warning, TEXT("Found sessions but they are invalid"));
		return;
	}
	/*
	Setup the server list
	*/
	TArray<FServerData> ServerNames;
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		bool IsThisFriendsOnlySession;
		if (SearchResult.Session.SessionSettings.Get(FRIENDS_ONLY_SETTINGS_KEY, IsThisFriendsOnlySession))
		{
			bool IsHostFriend = FriendsInterface->IsFriend(0, *SearchResult.Session.OwningUserId, FString());
			if (IsThisFriendsOnlySession == true && IsHostFriend == false)
			{
				continue;
			}
		}
		else
		{
			continue;
		}

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
		UE_LOG(LogOnline, Error, TEXT("Could not resolve connect string"));
		return;
	}
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	StartLoading();
	SetLoadingTitle(TEXT("Loading Game"));
	SetLoadingSubtitle(TEXT("Traveling to host."));

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UWildOmissionGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	//TODO goto main menu
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 20.0f, FColor::Red, FString::Printf(TEXT("Error String %s"), *ErrorString));
	UE_LOG(LogOnline, Error, TEXT("Network failure disconnecting..."));
	UE_LOG(LogOnline, Error, TEXT("Damnit larch you forgot to add disconnecting"));
}

IOnlineFriendsPtr UWildOmissionGameInstance::GetFriendsInterface() const
{
	return FriendsInterface;
}

FString UWildOmissionGameInstance::GetVersion() const
{
	return GameVersion;
}

UMainMenuWidget* UWildOmissionGameInstance::GetMainMenuWidget() const
{
	return MainMenuWidget;
}

UGameplayMenuWidget* UWildOmissionGameInstance::GetGameplayMenuWidget() const
{
	return GameplayMenuWidget;
}
