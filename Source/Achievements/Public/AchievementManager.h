// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineStats.h"
#include "AchievementManager.generated.h"

/**
 * 
 */
UCLASS()
class ACHIEVEMENTS_API UAchievementManager : public UObject
{
	GENERATED_BODY()

public:
	UAchievementManager();
	
	void OnCreation();
	virtual void BeginDestroy() override;

	static UAchievementManager* GetAchievementManager();

	void QueryAchievements();
	void UnlockAchievement(const FString& AchievementID);
private:
	FOnlineAchievementsWritePtr AchievementsWriteObjectPtr;
	
	void OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	void OnWriteAchievemetnsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	
};
