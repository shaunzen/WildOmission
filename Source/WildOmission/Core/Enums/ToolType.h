// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ToolType.generated.h"

UENUM()
enum EToolType
{
	WOOD	UMETA(DisplayName = "Wood"),
	STONE	UMETA(DisplayName = "Stone"),
	FABRIC	UMETA(DisplayName = "Fabric")
};