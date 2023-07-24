// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "AnimalSpawnData.generated.h"

class AAnimal;

USTRUCT(BlueprintType)
struct FAnimalSpawnData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAnimal> Class;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnGroupSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnWeight;

	FAnimalSpawnData();

};