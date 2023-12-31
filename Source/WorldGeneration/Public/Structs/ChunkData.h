// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/ActorSaveData.h"
#include "ChunkData.generated.h"

USTRUCT()
struct WORLDGENERATION_API FChunkData
{
	GENERATED_BODY()

	FChunkData();
	FChunkData(const FIntVector2& InGridLocation);

	UPROPERTY()
	FIntVector2 GridLocation;

	UPROPERTY()
	bool Generated;

	UPROPERTY()
	TArray<float> HeightData;

	UPROPERTY()
	TArray<uint8> SurfaceData;

	UPROPERTY()
	TArray<uint8> ByteData;

	UPROPERTY()
	TArray<FActorSaveData> ActorData;

	bool operator==(const FChunkData& Other) const
	{
		return this->GridLocation.X == Other.GridLocation.X && this->GridLocation.Y == Other.GridLocation.Y;
	}

};

uint32 WORLDGENERATION_API GetTypeHash(const FChunkData& ChunkData);