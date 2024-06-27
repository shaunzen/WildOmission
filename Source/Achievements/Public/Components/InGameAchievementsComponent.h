// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/AchievementStatsData.h"
#include "InGameAchievementsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACHIEVEMENTS_API UInGameAchievementsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInGameAchievementsComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UnlockAchievement(const FString& AchievementID);

	FAchievementStatsData GetStatsData() const;
	void SetStatsData(const FAchievementStatsData& InStatsData);

	void SetAchievementsEnabled(bool Enabled);
	bool AreAchievementsEnabled() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	FAchievementStatsData StatsData;

private:
	UFUNCTION(Client, Reliable)
	void Client_UnlockAchievement(const FString& AchievementID);

	UPROPERTY()
	bool AchievementsEnabled;

};
