// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/ItemData.h"
#include "CraftingRecipeEntry.generated.h"

USTRUCT()
struct FCraftingRecipeEntry
{
	GENERATED_BODY()

	FName RecipeID = FName();
	bool CanCraft = false;
	int32 IngredientPercentage = 0;
	int32 SortPriority = 0;
	FItemData* YieldItemData = nullptr;

	FORCEINLINE bool operator<(const FCraftingRecipeEntry& Other) const
	{
		return this->CanCraft > Other.CanCraft || this->IngredientPercentage > Other.IngredientPercentage;
	}
};
