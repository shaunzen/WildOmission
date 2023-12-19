// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionSaveGame.h"

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
	LevelFile = TEXT("LV_Procedural");
	Seed = 0;
	Version = 0;
	CheatsEnabled = false;
}