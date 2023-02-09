// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "FoodItem.generated.h"

class USoundBase;

UCLASS()
class WILDOMISSION_API AFoodItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	
	AFoodItem();

	virtual void Primary() override;

	uint8 GetEnergy();
	uint8 GetHydration();

	void SetEnergy(uint8 InEnergy);
	void SetHydration(uint8 InHydration);

private:
	UPROPERTY(EditDefaultsOnly)
	uint8 Energy;

	UPROPERTY(EditDefaultsOnly)
	uint8 Hydration;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ConsumptionSound;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayConsumeSound();

};
