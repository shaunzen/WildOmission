// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "ConsumableItem.generated.h"

class USoundBase;

UCLASS()
class WILDOMISSION_API AConsumableItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	virtual void OnPrimaryPressed() override;

private:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* ConsumptionSound;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayConsumeSound();

};
