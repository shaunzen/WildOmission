// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "FoodItem.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API AFoodItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	
	AFoodItem();

	virtual void Primary() override;

	uint8 GetEnergy();

	void SetEnergy(uint8 InEnergy);

private:
	UPROPERTY(EditAnywhere)
	uint8 Energy;

};
