// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventorySave.generated.h"

USTRUCT()
struct SAVESYSTEM_API FInventorySave
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<uint8> ByteData;
};