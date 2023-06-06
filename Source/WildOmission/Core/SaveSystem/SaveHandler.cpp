// Copyright Telephone Studios. All Rights Reserved.


#include "SaveHandler.h"
#include "WorldGenerationHandlerComponent.h"
#include "ActorSaveHandlerComponent.h"
#include "PlayerSaveHandlerComponent.h"
#include "WildOmission/Core/Structs/WorldGenerationSettings.h"
#include "WildOmissionSaveGame.h"
#include "WildOmission/Core/GameModes/WildOmissionGameMode.h"
#include "WildOmission/Weather/WeatherManager.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASaveHandler::ASaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WorldGenerationHandlerComponent = CreateDefaultSubobject<UWorldGenerationHandlerComponent>(FName("WorldGenerationHandlerComponent"));
	
	ActorSaveHandlerComponent = CreateDefaultSubobject<UActorSaveHandlerComponent>(FName("ActorSaveHandlerComponent"));

	PlayerSaveHandlerComponent = CreateDefaultSubobject<UPlayerSaveHandlerComponent>(FName("PlayerSaveHandlerComponent"));

}

void ASaveHandler::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UWildOmissionGameInstance>(GetWorld()->GetGameInstance());

	FTimerHandle AutoSaveTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AutoSaveTimerHandle, this, &ASaveHandler::SaveGame, 90.0f, true);

	FTimerHandle RegenerationTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RegenerationTimerHandle, this, &ASaveHandler::RegenerateResources, 120.0f, true);
}

void ASaveHandler::SaveGame()
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Aborting save, SaveFile was nullptr."));
		return;
	}
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Aborting save, couldn't get gamemode."));
		return;
	}

	SaveFile->LastPlayedTime = FDateTime::Now();
	if (GameMode->GetWeatherManager())
	{
		SaveFile->WeatherManagerSave.NextStormSpawnTime = GameMode->GetWeatherManager()->GetNextStormSpawnTime();
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
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	if (SaveFile->CreationInformation.LevelHasGenerated == false)
	{
		GameInstance->SetLoadingSubtitle(FString("Generating level."));
		GenerateLevel(SaveFile);
		
		UpdateSaveFile(SaveFile);
		return;
	}

	if (GameMode->GetWeatherManager())
	{
		GameMode->GetWeatherManager()->SetNextStormSpawnTime(SaveFile->WeatherManagerSave.NextStormSpawnTime);
	}

	GameInstance->SetLoadingSubtitle(FString("Loading objects."));
	ActorSaveHandlerComponent->LoadActors(SaveFile->ActorSaves);

	FTimerHandle ActorLoadedTimerHandle;
	FTimerDelegate ActorLoadedDelegate;
	ActorLoadedDelegate.BindUObject(GameInstance, &UWildOmissionGameInstance::StopLoading);
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

UWorldGenerationHandlerComponent* ASaveHandler::GetWorldGenerationHandler() const
{
	return WorldGenerationHandlerComponent;
}

void ASaveHandler::ValidateSave()
{
	if (CurrentSaveFileName.Len() > 0)
	{
		return;
	}

	CurrentSaveFileName = FString("PIE_Save");
	GameInstance->CreateWorld(CurrentSaveFileName);
}

void ASaveHandler::GenerateLevel(UWildOmissionSaveGame* SaveToModify)
{
	FWorldGenerationSettings GenerationSettings;
	FTimerHandle WorldGenerationTimerHandle;
	FTimerDelegate WorldGenerationTimerDelegate;

	WorldGenerationTimerDelegate.BindUFunction(WorldGenerationHandlerComponent, FName("Generate"), GenerationSettings, SaveToModify);

	GetWorld()->GetTimerManager().SetTimer(WorldGenerationTimerHandle, WorldGenerationTimerDelegate, 1.0f, false);
	
	SaveToModify->CreationInformation.LevelHasGenerated = true;
}

void ASaveHandler::RegenerateResources()
{
	FWorldGenerationSettings GenerationSettings;

	TArray<AActor*> AllNodesInWorld;
	TArray<AActor*> AllCollectablesInWorld;
	
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Node"), AllNodesInWorld);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Collectable"), AllCollectablesInWorld);

	if (AllNodesInWorld.Num() > GenerationSettings.MinNodeCount || AllCollectablesInWorld.Num() > GenerationSettings.MinCollectableCount)
	{
		return;
	}

	WorldGenerationHandlerComponent->RegenerateResources(GenerationSettings);
}

void ASaveHandler::UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile)
{
	if (UpdatedSaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Aborting update to save file, updated save file passed in was a nullptr."));
		return;
	}

	UGameplayStatics::SaveGameToSlot(UpdatedSaveFile, CurrentSaveFileName, 0);
}