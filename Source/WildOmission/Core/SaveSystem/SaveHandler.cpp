// Copyright Telephone Studios. All Rights Reserved.


#include "SaveHandler.h"
#include "PlayerSaveHandlerComponent.h"
#include "WorldItemSaveHandlerComponent.h"
#include "HarvestableSaveHandlerComponent.h"
#include "WildOmission/Core/Structs/WorldGenerationSettings.h"
#include "WildOmissionSaveGame.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASaveHandler::ASaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PlayerSaveHandlerComponent = CreateDefaultSubobject<UPlayerSaveHandlerComponent>(FName("PlayerSaveHandlerComponent"));
	WorldItemSaveHandlerComponent = CreateDefaultSubobject<UWorldItemSaveHandlerComponent>(FName("WorldItemSaveHandlerComponent"));
	HarvestableSaveHandlerComponent = CreateDefaultSubobject<UHarvestableSaveHandlerComponent>(FName("HarvestableSaveHandlerComponent"));
}

void ASaveHandler::BeginPlay()
{
	Super::BeginPlay();

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

	PlayerSaveHandlerComponent->Save(SaveFile->PlayerSaves);
	WorldItemSaveHandlerComponent->Save(SaveFile->WorldItems);
	HarvestableSaveHandlerComponent->Save(SaveFile->Harvestables);

	UpdateSaveFile(SaveFile);
}


void ASaveHandler::LoadGame(const FString& SaveFileName)
{
	CurrentSaveFileName = SaveFileName;
	
	ValidateSave();

	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		return;
	}

	if (SaveFile->CreationInformation.LevelHasGenerated == false)
	{
		GenerateLevel(SaveFile);
		UpdateSaveFile(SaveFile);
		return;
	}

	WorldItemSaveHandlerComponent->Load(SaveFile->WorldItems);
	HarvestableSaveHandlerComponent->Load(SaveFile->Harvestables);
}

UWildOmissionSaveGame* ASaveHandler::GetSaveFile()
{
	UWildOmissionSaveGame* SaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::CreateSaveGameObject(UWildOmissionSaveGame::StaticClass()));
	SaveFile = Cast<UWildOmissionSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveFileName, 0));

	return SaveFile;
}

UPlayerSaveHandlerComponent* ASaveHandler::GetPlayerHandler()
{
	return PlayerSaveHandlerComponent;
}

void ASaveHandler::ValidateSave()
{
	if (CurrentSaveFileName.Len() > 0)
	{
		return;
	}

	CurrentSaveFileName = FString("PIE_Save");
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	GameInstance->CreateSave(CurrentSaveFileName);
}

void ASaveHandler::GenerateLevel(UWildOmissionSaveGame* SaveToModify)
{
	FWorldGenerationSettings GenerationSettings;

	HarvestableSaveHandlerComponent->Generate(GenerationSettings);

	SaveToModify->CreationInformation.LevelHasGenerated = true;

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