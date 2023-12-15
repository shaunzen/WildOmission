// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/SpawnedChunkData.h"

FSpawnedChunkData::FSpawnedChunkData()
{
	GridLocation = FIntVector2(0, 0);
	Chunk = nullptr;
}

uint32 GetTypeHash(const FSpawnedChunkData& SpawnedChunkData)
{
	return FCrc::MemCrc32(&SpawnedChunkData.GridLocation, sizeof(FIntVector2));
}