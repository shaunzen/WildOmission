// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryItem.h"
#include "UObject/NoExportTypes.h"
#include "CraftingRecipe.generated.h"


USTRUCT(BlueprintType)
struct FCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryItem Yeild;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CraftTime = 0;

};