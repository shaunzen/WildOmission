// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerSpawnChunk.generated.h"

USTRUCT()
struct WORLDGENERATION_API FPlayerSpawnChunk
{
	GENERATED_BODY()

	FPlayerSpawnChunk();

	UPROPERTY()
	FIntVector2 ChunkLocation;

	UPROPERTY()
	bool IsSet;

};