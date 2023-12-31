// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/ChunkData.h"

FChunkData::FChunkData()
{
	GridLocation = FIntVector2();
	HeightData = TArray<float>();
	SurfaceData = TArray<uint8>();
	ByteData = TArray<uint8>();
	ActorData = TArray<FActorSaveData>();
}

FChunkData::FChunkData(const FIntVector2& InGridLocation)
{
	GridLocation = InGridLocation;
	HeightData = TArray<float>();
	SurfaceData = TArray<uint8>();
	ByteData = TArray<uint8>();
	ActorData = TArray<FActorSaveData>();
}

uint32 GetTypeHash(const FChunkData& ChunkData)
{
	return FCrc::MemCrc32(&ChunkData.GridLocation, sizeof(FIntVector2));
}
