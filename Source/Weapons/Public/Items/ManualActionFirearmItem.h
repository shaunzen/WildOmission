// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/FirearmItem.h"
#include "ManualActionFirearmItem.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONS_API AManualActionFirearmItem : public AFirearmItem
{
	GENERATED_BODY()

public:
	AManualActionFirearmItem();

	virtual void OnPrimaryPressed() override;
	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance) override;

	virtual void OnChamberedAnimationClimax(bool FromFirstPersonInstance) override;

private:
	UPROPERTY()
	bool Chambered;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ChamberMontage;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ChamberItemMontage;

};
