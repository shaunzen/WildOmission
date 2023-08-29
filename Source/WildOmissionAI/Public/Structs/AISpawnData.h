// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "AISpawnData.generated.h"

class AWildOmissionAICharacter;

USTRUCT(BlueprintType)
struct FAISpawnData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWildOmissionAICharacter> Class;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnGroupSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnWeight;

	FAISpawnData();

};