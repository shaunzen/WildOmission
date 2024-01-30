// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/InGameAchievementsComponent.h"
#include "Structs/AchievementStatsData.h"
#include "WOInGameAchievementsComponent.generated.h"

UCLASS()
class WILDOMISSIONCORE_API UWOInGameAchievementsComponent : public UInGameAchievementsComponent
{
	GENERATED_BODY()
	
public:
	UWOInGameAchievementsComponent();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOwnerPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void GiveDeathAchievement();

	UFUNCTION()
	void OnItemHarvested(const struct FInventoryItem& ItemHarvested);

	UFUNCTION()
	void CheckCraftAchievementConditions(const FName& ItemID);

};
