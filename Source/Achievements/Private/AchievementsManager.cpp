// Copyright Telephone Studios. All Rights Reserved.


#include "AchievementsManager.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Log.h"

static UAchievementsManager* Instance = nullptr;

UAchievementsManager::UAchievementsManager()
{
	AchievementsWriteObjectPtr = nullptr;
}

void UAchievementsManager::OnCreation()
{
	Instance = this;

	QueryAchievements();
}

void UAchievementsManager::BeginDestroy()
{
	Super::BeginDestroy();

	Instance = nullptr;
}

UAchievementsManager* UAchievementsManager::GetAchievementsManager()
{
	return Instance;
}

void UAchievementsManager::QueryAchievements()
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
	AchievementsInterface->QueryAchievements(*UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UAchievementsManager::OnQueryAchievementsComplete));
}

void UAchievementsManager::UnlockAchievement(const FString& AchievementID)
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
	AchievementsWriteObjectPtr->SetFloatStat(*AchievementID, 1.0f);

	// Write the achievements progress
	FOnlineAchievementsWriteRef AchievementsWriteObjectRef = AchievementsWriteObjectPtr.ToSharedRef();
	AchievementsInterface->WriteAchievements(*UserId, AchievementsWriteObjectRef, FOnAchievementsWrittenDelegate::CreateUObject(this, &UAchievementsManager::OnWriteAchievementsComplete));

	// TODO remove this after achievements are completed
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString::Printf(TEXT("Writing Achievement %s"), *AchievementID));
}

void UAchievementsManager::OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	FString Display = FString::Printf(TEXT("QueryAchievementsComplete UniqueId %s, Success %i"), *PlayerId.ToString(), bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, Display);

	UnlockAchievement(TEXT("ACH_OPEN_GAME"));
}

void UAchievementsManager::OnWriteAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	FString Display = FString::Printf(TEXT("WriteAchievementsComplete, UniqueId %s, Success %i"), *PlayerId.ToString(), bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, Display);
}
