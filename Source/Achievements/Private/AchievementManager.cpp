// Copyright Telephone Studios. All Rights Reserved.


#include "AchievementManager.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Log.h"

static UAchievementManager* Instance = nullptr;

UAchievementManager::UAchievementManager()
{
}

void UAchievementManager::OnCreation()
{
	Instance = this;

	WriteAchievement(TEXT("ACH_OPEN_GAME"), 100.0f);
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

void UAchievementManager::WriteAchievement(const FName& AchievementName, float Progress)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem == nullptr)
	{
		return;
	}

	IOnlineIdentityPtr IdentityInterface = Subsystem->GetIdentityInterface();
	IOnlineAchievementsPtr AchievementsInterface = Subsystem->GetAchievementsInterface();

	if (!AchievementsInterface.IsValid() || !IdentityInterface.IsValid())
	{
		UE_LOG(LogAchievements, Error, TEXT("UAchievementManager::UnlockAchievement, AchievementsInterface not valid!"));
		return;
	}

	TSharedPtr<const FUniqueNetId> LocalUserId = IdentityInterface->GetUniquePlayerId(0);

	FOnlineAchievementsWriteRef Achievement = MakeShareable(new FOnlineAchievementsWrite());
	Achievement->Properties.Add(AchievementName, Progress);

	AchievementsInterface->WriteAchievements(*LocalUserId, Achievement);
}
