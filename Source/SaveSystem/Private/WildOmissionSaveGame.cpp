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
	Class = nullptr;
}

FWeatherHandlerSave::FWeatherHandlerSave()
{
	NextStormSpawnTime = 0.0f;
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
	LevelFile = TEXT("LV_Sandbox");
	Version = 0;
	CheatsEnabled = false;
}