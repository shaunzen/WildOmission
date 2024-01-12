// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/SpawnedChunk.h"

FSpawnedChunk::FSpawnedChunk()
{
	GridLocation = FIntVector2(0, 0);
	Chunk = nullptr;
}

int32 FSpawnedChunk::Distance(const FIntVector2& OtherGridLocation) const
{
	int32 X = this->GridLocation.X - OtherGridLocation.X;
	int32 Y = this->GridLocation.Y - OtherGridLocation.Y;

	return (X * X + Y * Y) / 4;
}

uint32 GetTypeHash(const FSpawnedChunk& SpawnedChunk)
{
	return FCrc::MemCrc32(&SpawnedChunk.GridLocation, sizeof(FIntVector2));
}