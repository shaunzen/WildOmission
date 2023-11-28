// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameDifficulty.generated.h"

UENUM()
enum EGameDifficulty
{
	EGD_Peaceful, EGD_Easy, EGD_Normal, EGD_Hard
};