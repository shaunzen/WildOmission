// Copyright Telephone Studios. All Rights Reserved.


#include "SaveHandler.h"
#include "PlayerSaveHandlerComponent.h"
#include "WildOmissionSaveGame.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASaveHandler::ASaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PlayerSaveHandlerComponent = CreateDefaultSubobject<UPlayerSaveHandlerComponent>(FName("Player Save Handler Component"));

}

void ASaveHandler::SaveGame()
{
	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Aborting save, SaveFile was nullptr."));
		return;
	}

	PlayerSaveHandlerComponent->SavePlayers(SaveFile->PlayerSaves);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Saving the level"));
	UpdateSaveFile(SaveFile);
}


void ASaveHandler::LoadGame(const FString& SaveFileName)
{
	if (SaveFileName.Len() == 0)
	{
		CurrentSaveFileName = FString("PIE");
	}
	else
	{
		CurrentSaveFileName = SaveFileName;
	}

	UWildOmissionSaveGame* SaveFile = GetSaveFile();
	if (SaveFile == nullptr)
	{
		UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
		GameInstance->CreateSave(CurrentSaveFileName);
		SaveFile = GetSaveFile();
	}

	if (SaveFile->CreationInformation.LevelHasGenerated == false)
	{
		// TODO generate level
	}

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, CurrentSaveFileName);
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

void ASaveHandler::UpdateSaveFile(UWildOmissionSaveGame* UpdatedSaveFile)
{
	if (UpdatedSaveFile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Aborting update to save file, updated save file passed in was a nullptr."));
		return;
	}

	UGameplayStatics::SaveGameToSlot(UpdatedSaveFile, CurrentSaveFileName, 0);
}