// Copyright Telephone Studios. All Rights Reserved.


#include "AchievementManager.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Log.h"

static UAchievementManager* Instance = nullptr;

UAchievementManager::UAchievementManager()
{
}

void UAchievementManager::OnCreation()
{
	Instance = this;

	IncrementLeaderboardValue(TEXT("StoneCollected"));
	//QueryAchievements();
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
	// Get the online sub system
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem == nullptr)
	{
		return;
	}

	// Get the Identity from the sub system 
	// Meaning our player's profile and various online services
	IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
	if (!IdentityInterface.IsValid())
	{
		return;
	}

	// Get a thread-safe pointer (for more info check out this link: https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Templates/TSharedPtr/index.html )
	TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);

	// Get the achievements interface for this platform
	IOnlineAchievementsPtr AchievementsInterface = OnlineSubsystem->GetAchievementsInterface();

	if (!AchievementsInterface.IsValid())
	{
		return;
	}

	// Cache all the game's achievements for future use and bind the OnQueryAchievementsComplete function to fire when we're finished caching
	AchievementsInterface->QueryAchievements(*UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UAchievementManager::OnQueryAchievementsComplete));
}

// TODO will remove this function
void UAchievementManager::QueryLeaderboard()
{
	// Get the online sub system
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem == nullptr)
	{
		return;
	}

	// Get the Identity from the sub system 
	// Meaning our player's profile and various online services
	IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
	if (!IdentityInterface.IsValid())
	{
		return;
	}

	// Get a thread-safe pointer (for more info check out this link: https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Templates/TSharedPtr/index.html )
	TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);

	// Get the stats interface for this platform
	IOnlineLeaderboardsPtr LeaderboardsInterface = OnlineSubsystem->GetLeaderboardsInterface();
	if (!LeaderboardsInterface.IsValid())
	{
		return;
	}

	// Cache all the game's stats for future use and bind the OnQueryStatsComplete function to fire when we're finished caching
	TArray<FUniqueNetIdRef> Players{
		UserId.ToSharedRef()
	};
	FOnlineLeaderboardReadRef ReadObjectRef;
	LeaderboardsInterface->ReadLeaderboards(Players, ReadObjectRef);
	//LeaderboardsInterface->
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, TEXT("Begin Query Leaderboard %s"));
}

void UAchievementManager::UpdateAchievementProgress(const FString& Id, float Percent)
{
	// Get the online sub system
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem == nullptr)
	{
		return;
	}

	// Get the Identity from the sub system 
	// Meaning our player's profile and various online services
	IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();

	if (!IdentityInterface.IsValid())
	{
		return;
	}

	// Get a thread-safe pointer (for more info check out this link: https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Templates/TSharedPtr/index.html )
	TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);

	// Get the achievements interface for this platform
	IOnlineAchievementsPtr AchievementsInterface = OnlineSubsystem->GetAchievementsInterface();

	if (!AchievementsInterface.IsValid() && (AchievementsWriteObjectPtr.IsValid() || AchievementsWriteObjectPtr->WriteState != EOnlineAsyncTaskState::InProgress))
	{
		return;
	}

	// Make a shared pointer for achievement writing
	AchievementsWriteObjectPtr = MakeShareable(new FOnlineAchievementsWrite());

	// Sets the progress of the desired achievement - does nothing if the id is not valid
	AchievementsWriteObjectPtr->SetFloatStat(*Id, Percent);
	FOnAchievementsWrittenDelegate WrittenDelegate;
	WrittenDelegate.BindUObject(this, &UAchievementManager::OnUpdateAchievementsProgressComplete);
	// Write the achievements progress
	FOnlineAchievementsWriteRef AchievementsWriteObjectRef = AchievementsWriteObjectPtr.ToSharedRef();
	AchievementsInterface->WriteAchievements(*UserId, AchievementsWriteObjectRef);

	// TODO remove this after achievements are completed
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString::Printf(TEXT("Writing Achievement %s"), *Id));
}

void UAchievementManager::IncrementLeaderboardValue(const FString& Id)
{
	// Get the online sub system
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem == nullptr)
	{
		return;
	}

	// Get the Identity from the sub system 
	// Meaning our player's profile and various online services
	IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();

	if (!IdentityInterface.IsValid())
	{
		return;
	}

	// Get a thread-safe pointer (for more info check out this link: https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Templates/TSharedPtr/index.html )
	TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);

	// Get the achievements interface for this platform
	IOnlineLeaderboardsPtr LeaderboardInterface = OnlineSubsystem->GetLeaderboardsInterface();
	if (!LeaderboardInterface.IsValid())
	{
		return;
	}
	// So all this was not needed?
	/*TArray<FUniqueNetIdRef> Players{ UserId.ToSharedRef() };
	FOnlineLeaderboardReadRef LeaderboardReadRef;
	LeaderboardReadRef->LeaderboardName = *Id;
	LeaderboardInterface->ReadLeaderboards(Players, LeaderboardReadRef);
	FOnlineStatsRow* StatsRow = LeaderboardReadRef->FindPlayerRecord(*UserId);
	FVariantData StatValue = StatsRow->Columns.Find(*Id);*/

	FOnlineLeaderboardWrite LeaderboardWriteObject;
	LeaderboardWriteObject.IncrementIntStat(*Id);

	const bool Success = LeaderboardInterface->WriteLeaderboards(TEXT("Game"), *UserId, LeaderboardWriteObject);

	// TODO remove this after achievements are completed
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString::Printf(TEXT("Writing Leaderboard"), Success));
}

//void UAchievementManager::IncrementStatProgress(const FString& Id)
//{
//	// Get the online sub system
//	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
//
//	if (OnlineSubsystem == nullptr)
//	{
//		return;
//	}
//
//	// Get the Identity from the sub system 
//		// Meaning our player's profile and various online services
//	IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
//
//	if (!IdentityInterface.IsValid())
//	{
//		return;
//	}
//
//	// Get a thread-safe pointer
//	TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);
//
//	// Get the achievements interface for this platform
//	IOnlineStatsPtr StatsInterface = OnlineSubsystem->GetStatsInterface();
//	if (!StatsInterface.IsValid())
//	{
//		return;
//	}
//
//	// Somewhere in this absolute fucking mess of ABSOLUTE SHIT the stat is being incremented on valves servers??!!?!?
//	// Programmer at Epic says "how many hoops should the programmer jump through to increment a stat??"
//	// Another Programmer says "about 11, but make it more if you can"
//	FOnlineStatValue StatValue = UserStats->Stats.FindRef(Id);
//	
//	/*int64 ExistingValue = 0;
//	
//	StatValue.GetValue(ExistingValue);
//	StatValue.SetValue(ExistingValue + 1);*/
//
//	StatValue.SetValue(100);
//
//	FOnlineStatUpdate StatUpdate;
//	StatUpdate.Set(StatValue, FOnlineStatUpdate::EOnlineStatModificationType::Set);
//	
//	TMap<FString, FOnlineStatUpdate> StatMap;
//	StatMap.Add(Id, StatUpdate);
//
//	TArray<FOnlineStatsUserUpdatedStats> UpdatedStats;
//	UpdatedStats.Add(FOnlineStatsUserUpdatedStats(UserId.ToSharedRef(), StatMap));
//
//	StatsInterface->UpdateStats(UserId.ToSharedRef(), UpdatedStats, FOnlineStatsUpdateStatsComplete::CreateUObject(this, &UAchievementManager::OnUpdateStatComplete));
//
//	// TODO remove this after achievements are completed
//	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString::Printf(TEXT("Increment %s"), *Id));
//}

void UAchievementManager::OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	FString Display = FString::Printf(TEXT("QueryAchievementsComplete UniqueId %s, Success %i"), *PlayerId.ToString(), bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, Display);

	UpdateAchievementProgress(TEXT("StoneCollected"), 0.5f);
	UpdateAchievementProgress(TEXT("ACH_OPEN_GAME"), 100.0f);
}

void UAchievementManager::OnUpdateAchievementsProgressComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	FString Display = FString::Printf(TEXT("WriteAchievementsComplete, UniqueId %s, Success %i"), *PlayerId.ToString(), bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, Display);
}
