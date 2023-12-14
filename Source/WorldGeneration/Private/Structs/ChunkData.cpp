// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/ChunkData.h"

FChunkData::FChunkData()
{
	GridLocation = FIntVector2();
	ByteData = TArray<uint8>();
	ActorData = TArray<FActorSaveData>();
}

uint32 GetTypeHash(const FChunkData& ChunkData)
{
	return FCrc::MemCrc32(&ChunkData, sizeof(FChunkData));
}
