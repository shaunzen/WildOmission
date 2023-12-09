// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionSaveGame.h"

FActorComponentSaveData::FActorComponentSaveData()
{
	Name = NAME_None;
	Class = nullptr;
}

FActorSaveData::FActorSaveData()
{
	Identifier = NAME_None;
	Transform = FTransform::Identity;
}

FWildOmissionSaveCreationInformation::FWildOmissionSaveCreationInformation()
{
	Name = TEXT("");
	LevelHasGenerated = false;
	Day = 1;
	Month = 1;
	Year = 2023;
}

UWildOmissionSaveGame::UWildOmissionSaveGame()
{
	DaysPlayed = 0;
	NormalizedProgressThroughDay = 0.0f;
	LastPlayedTime = FDateTime::Now();
	Difficulty = EGameDifficulty::EGD_Normal;
	LevelFile = TEXT("LV_Sandbox");
	Version = 0;
	CheatsEnabled = false;
}

FChunkSaveData::FChunkSaveData()
{
	ByteData = TArray<uint8>();
	ActorData = TArray<FActorSaveData>();
}
