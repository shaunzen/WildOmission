// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/SaveHandler.h"
#include "Components/ActorSaveHandlerComponent.h"
#include "Components/PlayerSaveHandlerComponent.h"
#include "Actors/WorldGenerationHandler.h"
#include "TimeOfDayHandler.h"
#include "Interfaces/GameSaveLoadController.h"
#include "WildOmissionSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

// Sets default values
ASaveHandler::ASaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ActorSaveHandlerComponent = CreateDefaultSubobject<UActorSaveHandlerComponent>(FName("ActorSaveHandlerComponent"));
	PlayerSaveHandlerComponent = CreateDefaultSubobject<UPlayerSaveHandlerComponent>(FName("PlayerSaveHandlerComponent"));

	GameSaveLoadController = nullptr;
}

void ASaveHandler::Setup(IGameSaveLoadController* SaveLoadController)
{
	GameSaveLoadController = SaveLoadController;

	FTimerHandle AutoSaveTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AutoSaveTimerHandle, this, &ASaveHandler::SaveGame, 90.0f, true);
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
		GameSaveLoadController->SetLoadingSubtitle(FString("Generating level."));
		WorldGenerationHandler->GenerateLevel();
		SaveFile->CreationInformation.LevelHasGenerated = true;
		UpdateSaveFile(SaveFile);
		return;
	}

	ATimeOfDayHandler* TimeOfDayHandler = ATimeOfDayHandler::GetTimeOfDayHandler();
	if (TimeOfDayHandler)
	{
		TimeOfDayHandler->SetDaysPlayed(SaveFile->DaysPlayed);
		TimeOfDayHandler->SetNormalizedProgressThroughDay(SaveFile->NormalizedProgressThroughDay);
	}

	GameSaveLoadController->SetLoadingSubtitle(FString("Loading objects."));
	ActorSaveHandlerComponent->LoadActors(SaveFile->ActorSaves, SaveFile->Version);

	FTimerHandle ActorLoadedTimerHandle;
	FTimerDelegate ActorLoadedDelegate;
	ActorLoadedDelegate.BindUObject(this, &ASaveHandler::StopLoading);
	GetWorld()->GetTimerManager().SetTimer(ActorLoadedTimerHandle, ActorLoadedDelegate, 1.0f, false);
}

UWildOmissionSaveGame* ASaveHandler::GetSaveFile()
{
	UWildOmissionSaveGame* SaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
	SaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveFileName, 0));

	return SaveFile;
}

UPlayerSaveHandlerComponent* ASaveHandler::GetPlayerHandler() const
{
	return PlayerSaveHandlerComponent;
}

IGameSaveLoadController* ASaveHandler::GetSaveLoadController() const
{
	return GameSaveLoadController;
}

void ASaveHandler::ValidateSave()
{
	if (CurrentSaveFileName.Len() > 0 || GameSaveLoadController == nullptr)
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("Failed to validate save file, can't find GameSaveLoadController."));
		return;
	}

	CurrentSaveFileName = TEXT("PIE_Save");
	GameSaveLoadController->CreateWorld(CurrentSaveFileName);
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

void ASaveHandler::StopLoading()
{
	GameSaveLoadController->StopLoading();
}
