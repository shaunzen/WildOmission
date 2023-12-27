// Copyright Telephone Studios. All Rights Reserved.


#include "SaveManager.h"
#include "Components/PlayerSaveManagerComponent.h"
#include "ChunkManager.h"
#include "TimeOfDayManager.h"
#include "Interfaces/GameSaveLoadController.h"
#include "WildOmissionSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

static ASaveManager* Instance = nullptr;

// Sets default values
ASaveManager::ASaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GameSaveLoadController = nullptr;
	CurrentSaveFile = nullptr;

	PlayerSaveManagerComponent = CreateDefaultSubobject<UPlayerSaveManagerComponent>(TEXT("PlayerSaveManagerComponent"));
}

void ASaveManager::SetGameSaveLoadController(IGameSaveLoadController* InGameSaveLoadController)
{
	GameSaveLoadController = InGameSaveLoadController;
}

ASaveManager* ASaveManager::GetSaveManager()
{
	return Instance;
}

void ASaveManager::SaveGame()
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Aborting save, SaveFile was nullptr."));
		return;
	}
	SaveFile->LastPlayedTime = FDateTime::Now();

	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager)
	{
		SaveFile->Seed = ChunkManager->GetGenerationSeed();
		SaveFile->ChunkData = ChunkManager->GetChunksData();
	}

	ATimeOfDayManager* TimeOfDayManager = ATimeOfDayManager::GetTimeOfDayManager();
	if (TimeOfDayManager)
	{
		SaveFile->DaysPlayed = TimeOfDayManager->GetDaysPlayed();
		SaveFile->NormalizedProgressThroughDay = TimeOfDayManager->GetNormalizedProgressThroughDay();
	}

	PlayerSaveManagerComponent->Save(SaveFile->PlayerData);
	
	SaveFile->Version = CURRENT_SAVE_FILE_VERSION;

	UpdateSaveFile(SaveFile);
}

void ASaveManager::SetSaveFile(const FString& SaveFileName)
{
	CurrentSaveFileName = SaveFileName;
	ValidateSave();

	CurrentSaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
	CurrentSaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveFileName, 0));
}

void ASaveManager::LoadWorld()
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		return;
	}
	if (SaveFile->CreationInformation.LevelHasGenerated == false)
	{
		SetLoadingSubtitle(TEXT("Generating level."));
		
		const uint32 Seed = FMath::RandRange(0, 999999999);
		AChunkManager::SetGenerationSeed(Seed);

		MarkSaveGenerated();
		return;
	}

	ATimeOfDayManager* TimeOfDayManager = ATimeOfDayManager::GetTimeOfDayManager();
	if (TimeOfDayManager)
	{
		TimeOfDayManager->SetDaysPlayed(SaveFile->DaysPlayed);
		TimeOfDayManager->SetNormalizedProgressThroughDay(SaveFile->NormalizedProgressThroughDay);
	}
	
	SetLoadingSubtitle(TEXT("Loading level."));

	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager)
	{
		ChunkManager->SetGenerationSeed(SaveFile->Seed);
		ChunkManager->SetChunkData(SaveFile->ChunkData);
	}

	FTimerHandle FinishedLoadingTimerHandle;
	FTimerDelegate FinishedLoadingTimerDelegate;
	FinishedLoadingTimerDelegate.BindUObject(this, &ASaveManager::StopLoading);
	GetWorld()->GetTimerManager().SetTimer(FinishedLoadingTimerHandle, FinishedLoadingTimerDelegate, 1.0f, false);
}

UWildOmissionSaveGame* ASaveManager::GetSaveFile() const
{
	return CurrentSaveFile;
}

void ASaveManager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr || World->IsEditorWorld() && IsValid(Instance))
	{
		return;
	}

	Instance = this;

	FTimerHandle AutoSaveTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AutoSaveTimerHandle, this, &ASaveManager::SaveGame, 90.0f, true);
}

void ASaveManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Instance = nullptr;
}

UPlayerSaveManagerComponent* ASaveManager::GetPlayerManager() const
{
	return PlayerSaveManagerComponent;
}

void ASaveManager::ValidateSave()
{
	if (CurrentSaveFileName.Len() > 0)
	{
		UE_LOG(LogSaveSystem, Display, TEXT("Loading into valid save file: %s."), *CurrentSaveFileName);
		return;
	}

	CurrentSaveFileName = TEXT("PIE_Save");
	CreateWorld(CurrentSaveFileName);
}

void ASaveManager::MarkSaveGenerated()
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		return;
	}

	SaveFile->CreationInformation.LevelHasGenerated = true;
	UpdateSaveFile(SaveFile);
	
	FTimerHandle FinishedLoadingTimerHandle;
	FTimerDelegate FinishedLoadingTimerDelegate;
	FinishedLoadingTimerDelegate.BindUObject(this, &ASaveManager::StopLoading);
	GetWorld()->GetTimerManager().SetTimer(FinishedLoadingTimerHandle, FinishedLoadingTimerDelegate, 1.0f, false);
}

void ASaveManager::UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile)
{
	if (UpdatedSaveFile == nullptr)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Aborting update to save file, updated save file passed in was a nullptr."));
		return;
	}

	UGameplayStatics::SaveGameToSlot(UpdatedSaveFile, CurrentSaveFileName, 0);
}

void ASaveManager::StartLoading()
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->StartLoading();
}

void ASaveManager::StopLoading()
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->StopLoading();
}

void ASaveManager::SetLoadingTitle(const FString& NewTitle)
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->SetLoadingTitle(NewTitle);
}

void ASaveManager::SetLoadingSubtitle(const FString& NewSubtitle)
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->SetLoadingSubtitle(NewSubtitle);
}

void ASaveManager::CreateWorld(const FString& NewWorldName)
{
	if (GameSaveLoadController == nullptr)
	{
		return;
	}

	GameSaveLoadController->CreateWorld(NewWorldName);
}
