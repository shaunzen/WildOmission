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

	QueryAchievements();
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

void UAchievementManager::QueryAchievements()
{
	//Get the online sub system
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//Get the Identity from the sub system 
		//Meaning our player's profile and various online services
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();

		if (Identity.IsValid())
		{
			//Get a thread-safe pointer (for more info check out this link: https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Templates/TSharedPtr/index.html )
			TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);

			//Get the achievements interface for this platform
			IOnlineAchievementsPtr Achievements = OnlineSub->GetAchievementsInterface();

			if (Achievements.IsValid())
			{
				//Cache all the game's achievements for future use and bind the OnQueryAchievementsComplete function to fire when we're finished caching
				Achievements->QueryAchievements(*UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UAchievementManager::OnQueryAchievementsComplete));
			}
		}
	}
}

void UAchievementManager::UpdateAchievementProgress(const FString& Id, float Percent)
{
	//Get the online sub system
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//Get the Identity from the sub system 
		//Meaning our player's profile and various online services
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();

		if (Identity.IsValid())
		{
			//Get a thread-safe pointer (for more info check out this link: https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Templates/TSharedPtr/index.html )
			TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);

			//Get the achievements interface for this platform
			IOnlineAchievementsPtr Achievements = OnlineSub->GetAchievementsInterface();


			if (Achievements.IsValid() && (!AchievementsWriteObjectPtr.IsValid() || !AchievementsWriteObjectPtr->WriteState != EOnlineAsyncTaskState::InProgress))
			{
				//Make a shared pointer for achievement writing
				AchievementsWriteObjectPtr = MakeShareable(new FOnlineAchievementsWrite());

				//Sets the progress of the desired achievement - does nothing if the id is not valid
				AchievementsWriteObjectPtr->SetFloatStat(*Id, Percent);
				FOnAchievementsWrittenDelegate WrittenDelegate;
				WrittenDelegate.BindUObject(this, &UAchievementManager::OnUpdateAchievementsProgressComplete);
				//Write the achievements progress
				FOnlineAchievementsWriteRef AchievementsWriteObjectRef = AchievementsWriteObjectPtr.ToSharedRef();
				Achievements->WriteAchievements(*UserId, AchievementsWriteObjectRef);
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString::Printf(TEXT("WriteAchievement %s, %f"), *Id, Percent));
}

void UAchievementManager::OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	FString Display = FString::Printf(TEXT("QueryAchievementsComplete UniqueId %s, Success %i"), *PlayerId.ToString(), bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, Display);

	UpdateAchievementProgress(TEXT("ACH_OPEN_GAME"), 100.0f);
}

void UAchievementManager::OnUpdateAchievementsProgressComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	FString Display = FString::Printf(TEXT("WriteAchievementsComplete, UniqueId %s, Success %i"), *PlayerId.ToString(), bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, Display);
}
