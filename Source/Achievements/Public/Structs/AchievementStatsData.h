// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AchievementStatsData.generated.h"

USTRUCT()
struct ACHIEVEMENTS_API FAchievementStatsData
{
	GENERATED_BODY()

	FAchievementStatsData();

	// TODO make one of these for all harvestable resources, just for the future in case we add more achievements
	// or just want to make a stat menu.

	UPROPERTY()
	int32 WoodHarvested;

	UPROPERTY()
	int32 StoneHarvested;

	UPROPERTY()
	int32 MetalOreHarvested;

	UPROPERTY()
	int32 SulfurOreHarvested;

	UPROPERTY()
	int32 RefinedMetalHarvested;

	UPROPERTY()
	int32 GoldHarvested;

	UPROPERTY()
	int32 LeatherHarvested;

	UPROPERTY()
	int32 BerriesHarvested;

	UPROPERTY()
	int32 ClothHarvested;

	UPROPERTY()
	int32 BuildingBlocksPlaced;

	UPROPERTY()
	int32 FoodCooked;

	UPROPERTY()
	int32 MetalSmelted;

	UPROPERTY()
	int32 SulfurSmelted;

	UPROPERTY()
	int32 RangedHeadshots;

	UPROPERTY()
	int32 RangedBodyshots;

	UPROPERTY()
	int32 MeleeHeadshots;

	UPROPERTY()
	int32 MeleeBodyshots;

};