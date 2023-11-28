// Copyright Telephone Studios. All Rights Reserved.


#include "SaveHandler.h"
#include "Components/ActorSaveHandlerComponent.h"
#include "Components/PlayerSaveHandlerComponent.h"
#include "WorldGenerationHandler.h"
#include "TimeOfDayHandler.h"
#include "Interfaces/GameSaveLoadController.h"
#include "WildOmissionSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

static ASaveHandler* Instance = nullptr;

// Sets default values
ASaveHandler::ASaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GameSaveLoadController = nullptr;
	CurrentSaveFile = nullptr;

	ActorSaveHandlerComponent = CreateDefaultSubobject<UActorSaveHandlerComponent>(TEXT("ActorSaveHandlerComponent"));
	PlayerSaveHandlerComponent = CreateDefaultSubobject<UPlayerSaveHandlerComponent>(TEXT("PlayerSaveHandlerComponent"));
}

void ASaveHandler::SetGameSaveLoadController(IGameSaveLoadController* InGameSaveLoadController)
{
	GameSaveLoadController = InGameSaveLoadController;
}

ASaveHandler* ASaveHandler::GetSaveHandler()
{
	return Instance;
}

void ASaveHandler::SaveGame()
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Aborting save, SaveFile was nullptr."));
		return;
	}
	SaveFile->LastPlayedTime = FDateTime::Now();

	ATimeOfDayHandler* TimeOfDayHandler = ATimeOfDayHandler::GetTimeOfDayHandler();
	if (TimeOfDayHandler)
	{
		SaveFile->DaysPlayed = TimeOfDayHandler->GetDaysPlayed();
		SaveFile->NormalizedProgressThroughDay = TimeOfDayHandler->GetNormalizedProgressThroughDay();
	}

	ActorSaveHandlerComponent->SaveActors(SaveFile->ActorSaves);
	PlayerSaveHandlerComponent->Save(SaveFile->PlayerSaves);
	
	SaveFile->Version = CURRENT_SAVE_FILE_VERSION;

	UpdateSaveFile(SaveFile);
}

void ASaveHandler::SetSaveFile(const FString& SaveFileName)
{
	CurrentSaveFileName = SaveFileName;
	ValidateSave();

	CurrentSaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
	CurrentSaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveFileName, 0));
}

void ASaveHandler::LoadWorld()
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		return;
	}

	AWorldGenerationHandler* WorldGenerationHandler = AWorldGenerationHandler::GetWorldGenerationHandler();
	if (WorldGenerationHandler && SaveFile->CreationInformation.LevelHasGenerated == false)
	{
		SetLoadingSubtitle(TEXT("Generating level."));
		WorldGenerationHandler->GenerateLevel();
		WorldGenerationHandler->OnGenerationComplete.AddDynamic(this, &ASaveHandler::MarkSaveGenerated);
		return;
	}

	ATimeOfDayHandler* TimeOfDayHandler = ATimeOfDayHandler::GetTimeOfDayHandler();
	if (TimeOfDayHandler)
	{
		TimeOfDayHandler->SetDaysPlayed(SaveFile->DaysPlayed);
		TimeOfDayHandler->SetNormalizedProgressThroughDay(SaveFile->NormalizedProgressThroughDay);
	}
	
	SetLoadingSubtitle(TEXT("Loading objects."));
	ActorSaveHandlerComponent->LoadActors(SaveFile->ActorSaves, SaveFile->Version);

	// Make sure there is at least 20 of all collectables
	WorldGenerationHandler->PreventExtinction();

	FTimerHandle ActorLoadedTimerHandle;
	FTimerDelegate ActorLoadedDelegate;
	ActorLoadedDelegate.BindUObject(this, &ASaveHandler::StopLoading);
	GetWorld()->GetTimerManager().SetTimer(ActorLoadedTimerHandle, ActorLoadedDelegate, 1.0f, false);
}

UWildOmissionSaveGame* ASaveHandler::GetSaveFile() const
{
	return CurrentSaveFile;
}

void ASaveHandler::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr || World->IsEditorWorld() && IsValid(Instance))
	{
		return;
	}

	Instance = this;

	FTimerHandle AutoSaveTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AutoSaveTimerHandle, this, &ASaveHandler::SaveGame, 90.0f, true);
}

void ASaveHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Instance = nullptr;
}

UPlayerSaveHandlerComponent* ASaveHandler::GetPlayerHandler() const
{
	return PlayerSaveHandlerComponent;
}

void ASaveHandler::ValidateSave()
{
	if (CurrentSaveFileName.Len() > 0)
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Loading into valid save file: %s."), *CurrentSaveFileName);
		return;
	}

	CurrentSaveFileName = TEXT("PIE_Save");
	CreateWorld(CurrentSaveFileName);
}

void ASaveHandler::MarkSaveGenerated()
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		return;
	}

	SaveFile->CreationInformation.LevelHasGenerated = true;
	UpdateSaveFile(SaveFile);
}

void ASaveHandler::UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile)
{
	if (UpdatedSaveFile == nullptr)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Aborting update to save file, updated save file passed in was a nullptr."));
		return;
	}

	UGameplayStatics::SaveGameToSlot(UpdatedSaveFile, CurrentSaveFileName, 0);
}

void ASaveHandler::StartLoading()
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->StartLoading();
}

void ASaveHandler::StopLoading()
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->StopLoading();
}

void ASaveHandler::SetLoadingTitle(const FString& NewTitle)
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->SetLoadingTitle(NewTitle);
}

void ASaveHandler::SetLoadingSubtitle(const FString& NewSubtitle)
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->SetLoadingSubtitle(NewSubtitle);
}

void ASaveHandler::CreateWorld(const FString& NewWorldName)
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->CreateWorld(NewWorldName);
}
