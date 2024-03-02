// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "Structs/InventoryItem.h"
#include "LootBarrel.generated.h"

UCLASS()
class DEPLOYABLES_API ALootBarrel : public ADeployable
{
	GENERATED_BODY()

public:
	ALootBarrel();

	virtual void OnDeployableDestroyed() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryItem> Drops;

	UFUNCTION()
	void DropLoot();
	
};
