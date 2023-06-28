// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerBase.h"
#include "Structs/InventoryItem.h"
#include "LootCrateBase.generated.h"

USTRUCT(BlueprintType)
struct DEPLOYABLES_API FLootCrateItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FInventoryItem Item;

	UPROPERTY(EditDefaultsOnly)
	float Probability; // 1 = Certain | 0.5 = Even | 0 = Impossible
};

UCLASS()
class DEPLOYABLES_API ALootCrateBase : public AItemContainerBase
{
	GENERATED_BODY()
public:
	ALootCrateBase();

protected:
	virtual void BeginPlay() override;
	virtual void OnContainerClosed() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FLootCrateItem> ItemsToDrop;
	
	UFUNCTION()
	void SpawnItems();

	FInventoryItem GetDrop();

};
