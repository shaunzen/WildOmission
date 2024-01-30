// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineStats.h"
#include "AchievementsManager.generated.h"

/**
 * 
 */
UCLASS()
class ACHIEVEMENTS_API UAchievementsManager : public UObject
{
	GENERATED_BODY()

public:
	UAchievementsManager();
	
	void OnCreation();
	virtual void BeginDestroy() override;

	static UAchievementsManager* GetAchievementsManager();

	void QueryAchievements();
	void UnlockAchievement(const FString& AchievementID);
private:
	FOnlineAchievementsWritePtr AchievementsWriteObjectPtr;
	
	void OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	void OnWriteAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	
};
