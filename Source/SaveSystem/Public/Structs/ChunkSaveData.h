// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/ActorSaveData.h"
#include "ChunkSaveData.generated.h"

USTRUCT()
struct FChunkSaveData
{
	GENERATED_BODY()

	FChunkSaveData();

	UPROPERTY()
	FIntVector2 GridLocation;

	UPROPERTY()
	TArray<uint8> ByteData;

	UPROPERTY()
	TArray<FActorSaveData> ActorData;

};