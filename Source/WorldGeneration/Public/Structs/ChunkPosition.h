// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChunkPosition.generated.h"

USTRUCT()
struct FChunkPosition
{
	GENERATED_BODY()

	FChunkPosition()
	{
		X = 0;
		Y = 0;
	}

	FChunkPosition(const int32& InX, const int32& InY)
	{
		X = InX;
		Y = InY;
	}

	UPROPERTY()
	int32 X = 0;

	UPROPERTY()
	int32 Y = 0;

	bool operator==(const FChunkPosition& Other) const
	{
		return (X == Other.X && Y == Other.Y);
	}
};