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

	UPROPERTY()
	int32 WoodCollected;

	UPROPERTY()
	int32 StoneCollected;

	UPROPERTY()
	int32 GoldCollected;

	UPROPERTY()
	int32 RefinedMetalCollected;

	UPROPERTY()
	int32 BuildingBlocksPlaced;

	UPROPERTY()
	int32 RangedHeadshots;

	UPROPERTY()
	int32 RangedBodyshots;

	UPROPERTY()
	int32 MeleeHeadshots;

	UPROPERTY()
	int32 MeleeBodyshots;

};