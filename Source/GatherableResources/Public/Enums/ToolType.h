// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ToolType.generated.h"

UENUM()
enum EToolType
{
	// Wood is for trees.
	WOOD	UMETA(DisplayName = "Wood"),
	// Stone is for nodes.
	STONE	UMETA(DisplayName = "Stone"),
	// Unused as far as I know.
	METAL	UMETA(DisplayName = "Metal"),
	// Multi is used by the rock to allow it to gather both node and wood resources.
	MULTI	UMETA(DisplayName = "Multi-Tool"),
	// Unassigned / Generic.
	NONE	UMETA(DisplayName = "None"),
	// Used to denote that it's for building purposes only
	// The building hammer uses this one.
	BUILD	UMETA(DisplayName = "Building Tool"),
	// Unused :3
	FABRIC	UMETA(DisplayName = "Fabric")
};