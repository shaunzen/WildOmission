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
	void GiveDeathAchievement(const FVector& DeathLocation);

	UFUNCTION()
	void OnKilledAnotherPlayer(APlayerController* KilledPlayer);

	UFUNCTION()
	void OnOpenContainerChanged(class UInventoryComponent* NewContainerInventory);

	UFUNCTION()
	void OnItemSmelted(const struct FInventoryItem& RawItem, const struct FInventoryItem& SmeltedItem);

	UFUNCTION()
	void OnItemEquiped(class AEquipableItem* NewItem);

	UFUNCTION()
	void OnItemHarvested(const struct FInventoryItem& ItemHarvested);

	UFUNCTION()
	void CheckCraftAchievementConditions(const FName& ItemID);

	UFUNCTION()
	void OnDeployablePlaced(class ADeployable* DeployablePlaced);

	UFUNCTION()
	void OnHitmarker(bool IsHeadshot);

};
