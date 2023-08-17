// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MonsterSpawnData.generated.h"

class AMonster;

USTRUCT(BlueprintType)
struct FMonsterSpawnData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMonster> Class;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnGroupSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnWeight;

	FMonsterSpawnData();

};