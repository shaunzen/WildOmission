// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineSubsystem.h"
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
	
	void OnCreation(IOnlineAchievementsPtr InAchievementsInterface);
	virtual void BeginDestroy() override;

	static UAchievementManager* GetAchievementManager();

	void UnlockAchievement();

private:
	IOnlineAchievementsPtr AchievementsInterface;

};
