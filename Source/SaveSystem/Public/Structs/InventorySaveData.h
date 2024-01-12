// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventorySaveData.generated.h"

USTRUCT()
struct SAVESYSTEM_API FInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<uint8> ByteData;
};