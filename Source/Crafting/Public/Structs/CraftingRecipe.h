// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Structs/InventoryItem.h"
#include "UObject/NoExportTypes.h"
#include "CraftingRecipe.generated.h"


USTRUCT(BlueprintType)
struct CRAFTING_API FCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 YieldQuantity = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CraftTime = 0;

};