// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Structs/PlayerSave.h"
#include "Enums/GameDifficulty.h"
#include "WildOmissionSaveGame.generated.h"

const int32 CURRENT_SAVE_FILE_VERSION = 1;

USTRUCT()
struct FActorComponentSaveData
{
	GENERATED_BODY()

	FActorComponentSaveData();

	UPROPERTY()
	FName Name;

	UPROPERTY()
	UClass* Class;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

	FActorSaveData();

	UPROPERTY()
	FName Identifier;
	
	UPROPERTY()
	FTransform Transform;
	
	UPROPERTY()
	TArray<uint8> ByteData;

	UPROPERTY()
	TArray<FActorComponentSaveData> ComponentData;

};

USTRUCT()
struct FChunkSaveData
{
	GENERATED_BODY()

	FChunkSaveData();

	UPROPERTY()
	TArray<uint8> ByteData;

	UPROPERTY()
	TArray<FActorSaveData> ActorData;

};

USTRUCT()
struct FWildOmissionSaveCreationInformation
{
	GENERATED_BODY()

	FWildOmissionSaveCreationInformation();

	UPROPERTY()
	FString Name;

	UPROPERTY()
	bool LevelHasGenerated;

	UPROPERTY()
	uint8 Day;
	
	UPROPERTY()
	uint8 Month;
	
	UPROPERTY()
	uint16 Year;

};

UCLASS()
class SAVESYSTEM_API UWildOmissionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UWildOmissionSaveGame();

	UPROPERTY()
	uint32 DaysPlayed;
	
	UPROPERTY()
	float NormalizedProgressThroughDay;

	UPROPERTY()
	FDateTime LastPlayedTime;

	UPROPERTY()
	TEnumAsByte<EGameDifficulty> Difficulty;

	UPROPERTY()
	FString LevelFile;

	UPROPERTY()
	FWildOmissionSaveCreationInformation CreationInformation;

	UPROPERTY()
	int32 Version;

	UPROPERTY()
	bool CheatsEnabled;

	//UPROPERTY()
	//TArray<FActorSaveData> ActorSaves;
	


	UPROPERTY()
	TArray<FPlayerSave> PlayerSaves;

};