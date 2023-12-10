// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/ChunkSaveData.h"

FChunkSaveData::FChunkSaveData()
{
	GridLocation = FIntVector2();
	ByteData = TArray<uint8>();
	ActorData = TArray<FActorSaveData>();
}
