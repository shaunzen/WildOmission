// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnedChunk.generated.h"

USTRUCT()
struct WORLDGENERATION_API FSpawnedChunk
{
	GENERATED_BODY()

	FSpawnedChunk();

	UPROPERTY()
	FIntVector2 GridLocation;

	UPROPERTY()
	class AChunk* Chunk;
	
	int32 Distance(const FIntVector2& OtherGridLocation) const;

	bool operator==(const FSpawnedChunk& Other) const
	{
		return this->GridLocation.X == Other.GridLocation.X && this->GridLocation.Y == Other.GridLocation.Y;
	}

};

uint32 WORLDGENERATION_API GetTypeHash(const FSpawnedChunk& SpawnedChunk);