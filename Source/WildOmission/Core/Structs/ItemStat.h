// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemStat.generated.h"

USTRUCT(BlueprintType)
struct FItemStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName Name = FName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Value = -1;
};