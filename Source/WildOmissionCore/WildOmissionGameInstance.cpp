// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/MainMenuWidget.h"
#include "UI/GameplayMenuWidget.h"
#include "UI/LoadingMenuWidget.h"
#include "WildOmissionSaveGame.h"
#include "WildOmissionGameUserSettings.h"
#include "GameFramework/PlayerState.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"

const static FString GameVersion = TEXT("Demo Preview 1.0.3");

static USoundMix* MasterSoundMixModifier = nullptr;
static USoundClass* MasterSoundClass = nullptr;

UWildOmissionGameInstance::UWildOmissionGameInstance(const FObjectInitializer& ObjectIntializer)
{
	LoadingMenuWidget = nullptr;
	MainMenuWidget = nullptr;
	GameplayMenuWidget = nullptr;
	Loading = false;
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
}

UWildOmissionGameInstance* UWildOmissionGameInstance::GetWildOmissionGameInstance(UWorld* WorldContextObject)
{
	return Cast<UWildOmissionGameInstance>(WorldContextObject->GetGameInstance());
}

void UWildOmissionGameInstance::Init()
{
	Super::Init();

	ApplyMasterVolume();
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UWildOmissionGameInstance::LoadedNewMap);
	RunAutoConfigQualitySettings();
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

	MainMenuWidget->Setup(this);
	OnMainMenu = true;
}

void UWildOmissionGameInstance::ShowGameplayMenuWidget()
{
	if (GameplayMenuWidgetBlueprintClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create the gameplay menu widget, blueprint class was nullptr"));
		return;
	}
	
	GameplayMenuWidget = CreateWidget<UGameplayMenuWidget>(this, GameplayMenuWidgetBlueprintClass);
	
	if (GameplayMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create gameplay menu widget"));
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

void UWildOmissionGameInstance::RunAutoConfigQualitySettings()
{
	UWildOmissionGameUserSettings* WOUserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (WOUserSettings == nullptr || WOUserSettings->GetHasRunAutoConfig())
	{
		return;
	}

	WOUserSettings->RunHardwareBenchmark();
	WOUserSettings->ApplyHardwareBenchmarkResults();

	WOUserSettings->SetHasRunAutoConfig(true);
	WOUserSettings->ApplySettings(false);
}

void UWildOmissionGameInstance::StartLoading()
{
	Loading = true;

	if (LoadingMenuWidgetBlueprintClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something went wrong displaying the loading screen."));
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

void UWildOmissionGameInstance::QuitToMenu()
{
	UE_LOG(LogTemp, Display, TEXT("Returning to main menu."));
	ReturnToMainMenu();
}

void UWildOmissionGameInstance::ApplyMasterVolume()
{
	UWildOmissionGameUserSettings* WOUserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMixModifier, MasterSoundClass, WOUserSettings->GetMasterVolume(), 1.0f, 0.2f);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMixModifier);
}

//****************************
// HOSTING/JOINING
//****************************
void UWildOmissionGameInstance::StartSingleplayer(const FString& WorldName)
{
	if (MainMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot start singleplayer, MainMenuWidget was nullptr."));
		return;
	}

	OnMainMenu = false;

	// Remove the menu from viewport
	MainMenuWidget->Teardown();

	// Show the loading menu
	StartLoading();
	SetLoadingTitle(FString("Loading Game"));
	SetLoadingSubtitle(FString("Loading level."));

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

void UWildOmissionGameInstance::LoadedNewMap(UWorld* InWorld)
{
	if (Loading == false)
	{
		return;
	}

	StartLoading();
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
