// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/SpawnedChunkData.h"

FSpawnedChunkData::FSpawnedChunkData()
{
	GridLocation = FIntVector2(0, 0);
	Chunk = nullptr;
}

int32 FSpawnedChunkData::Distance(const FIntVector2& OtherGridLocation) const
{
	int32 X = this->GridLocation.X - OtherGridLocation.X;
	int32 Y = this->GridLocation.Y - OtherGridLocation.Y;

	return FMath::Abs(X + Y);
}

uint32 GetTypeHash(const FSpawnedChunkData& SpawnedChunkData)
{
	return FCrc::MemCrc32(&SpawnedChunkData.GridLocation, sizeof(FIntVector2));
}