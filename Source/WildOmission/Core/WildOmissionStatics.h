// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/ItemData.h"
#include "WildOmission/UI/Custom/UIColor.h"
#include "WildOmissionStatics.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API UWildOmissionStatics : public UObject
{
	GENERATED_BODY()
public:

	UWildOmissionStatics();
	static FItemData* GetItemData(const FName& ItemName);
	static FUIColor* GetUIColor(const FName& ColorName);
	static FLinearColor GetHighlightedColor(FUIColor* Color);
	static FLinearColor GetSelectedColor(FUIColor* Color);
	static FVector GetHostLocationInWorld(UWorld* WorldContextObject);
	static void SpawnWorldItem(UWorld* WorldContextObject, const struct FInventoryItem& ItemToSpawn, AActor* SpawningActor);
	static void GetWorldSize(UWorld* WorldContextObject, FVector2D& OutWorldSize);
};
