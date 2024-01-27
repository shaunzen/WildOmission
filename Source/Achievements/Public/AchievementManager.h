// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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

	void WriteAchievement(const FName& AchievementName, float Progress);

private:

};
