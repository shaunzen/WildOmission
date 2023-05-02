// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/ItemData.h"
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
	static void SpawnWorldItem(UWorld* WorldContextObject, const struct FInventoryItem& ItemToSpawn, AActor* SpawningActor);
};
