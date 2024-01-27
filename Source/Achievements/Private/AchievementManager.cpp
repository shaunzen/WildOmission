// Copyright Telephone Studios. All Rights Reserved.


#include "AchievementManager.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Log.h"

static UAchievementManager* Instance = nullptr;

UAchievementManager::UAchievementManager()
{
	AchievementsInterface = nullptr;
}

void UAchievementManager::OnCreation(IOnlineAchievementsPtr InAchievementsInterface)
{
	AchievementsInterface = InAchievementsInterface;
	Instance = this;
}

void UAchievementManager::BeginDestroy()
{
	Super::BeginDestroy();

	Instance = nullptr;
}

UAchievementManager* UAchievementManager::GetAchievementManager()
{
	return Instance;
}

void UAchievementManager::UnlockAchievement()
{
	if (!AchievementsInterface.IsValid())
	{
		UE_LOG(LogAchievements, Error, TEXT("UAchievementManager::UnlockAchievement, AchievementsInterface not valid!"));
		return;
	}

	// TODO implement logic here
}
