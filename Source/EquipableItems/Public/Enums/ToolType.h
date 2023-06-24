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
	MULTI	UMETA(DisplayName = "Multi-Tool"),
	NONE	UMETA(DisplayName = "None"),
	BUILD	UMETA(DisplayName = "Building Tool"),
	FABRIC	UMETA(DisplayName = "Fabric")
};