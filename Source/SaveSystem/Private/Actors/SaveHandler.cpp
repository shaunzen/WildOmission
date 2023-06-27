// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/SaveHandler.h"
#include "Components/ActorSaveHandlerComponent.h"
#include "Components/PlayerSaveHandlerComponent.h"
//#include "WorldGenerationHandlerComponent.h"
//#include "WildOmission/Core/Structs/WorldGenerationSettings.h"
#include "Interfaces/GameSaveLoadController.h"
#include "WildOmissionSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

// Sets default values
ASaveHandler::ASaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//WorldGenerationHandlerComponent = CreateDefaultSubobject<UWorldGenerationHandlerComponent>(FName("WorldGenerationHandlerComponent"));
	GameSaveLoadController = nullptr;
	ActorSaveHandlerComponent = CreateDefaultSubobject<UActorSaveHandlerComponent>(FName("ActorSaveHandlerComponent"));
	PlayerSaveHandlerComponent = CreateDefaultSubobject<UPlayerSaveHandlerComponent>(FName("PlayerSaveHandlerComponent"));
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
		UE_LOG(LogTemp, Error, TEXT("Aborting save, SaveFile was nullptr."));
		return;
	}
	
	ActorSaveHandlerComponent->SaveActors(SaveFile->ActorSaves);
	PlayerSaveHandlerComponent->Save(SaveFile->PlayerSaves);

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

	if (SaveFile->CreationInformation.LevelHasGenerated == false)
	{
		GameSaveLoadController->SetLoadingSubtitle(FString("Generating level."));
		//GenerateLevel(SaveFile);
		
		UpdateSaveFile(SaveFile);
		return;
	}

	//if (GameMode->GetWeatherManager())
	//{
	//	GameMode->GetWeatherManager()->SetNextStormSpawnTime(SaveFile->WeatherManagerSave.NextStormSpawnTime);
	//}

	GameSaveLoadController->SetLoadingSubtitle(FString("Loading objects."));
	ActorSaveHandlerComponent->LoadActors(SaveFile->ActorSaves);

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

//UWorldGenerationHandlerComponent* ASaveHandler::GetWorldGenerationHandler() const
//{
//	return WorldGenerationHandlerComponent;
//}

void ASaveHandler::ValidateSave()
{
	if (CurrentSaveFileName.Len() > 0)
	{
		return;
	}

	CurrentSaveFileName = FString("PIE_Save");
	GameSaveLoadController->CreateWorld(CurrentSaveFileName);
}
//
//void ASaveHandler::GenerateLevel(UWildOmissionSaveGame* SaveToModify)
//{
//	FWorldGenerationSettings GenerationSettings;
//	FTimerHandle WorldGenerationTimerHandle;
//	FTimerDelegate WorldGenerationTimerDelegate;
//
//	WorldGenerationTimerDelegate.BindUFunction(WorldGenerationHandlerComponent, FName("Generate"), GenerationSettings, SaveToModify);
//
//	GetWorld()->GetTimerManager().SetTimer(WorldGenerationTimerHandle, WorldGenerationTimerDelegate, 1.0f, false);
//	
//	SaveToModify->CreationInformation.LevelHasGenerated = true;
//}
//
//void ASaveHandler::RegenerateResources()
//{
//	FWorldGenerationSettings GenerationSettings;
//
//	TArray<AActor*> AllNodesInWorld;
//	TArray<AActor*> AllCollectablesInWorld;
//	
//	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Node"), AllNodesInWorld);
//	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Collectable"), AllCollectablesInWorld);
//
//	if (AllNodesInWorld.Num() > GenerationSettings.MinNodeCount || AllCollectablesInWorld.Num() > GenerationSettings.MinCollectableCount)
//	{
//		return;
//	}
//
//	WorldGenerationHandlerComponent->RegenerateResources(GenerationSettings);
//}

void ASaveHandler::UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile)
{
	if (UpdatedSaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Aborting update to save file, updated save file passed in was a nullptr."));
		return;
	}

	UGameplayStatics::SaveGameToSlot(UpdatedSaveFile, CurrentSaveFileName, 0);
}

void ASaveHandler::StopLoading()
{
	GameSaveLoadController->StopLoading();
}
