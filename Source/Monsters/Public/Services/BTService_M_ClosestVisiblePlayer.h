// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Services/BTService_ClosestVisiblePlayer.h"
#include "BTService_M_ClosestVisiblePlayer.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API UBTService_M_ClosestVisiblePlayer : public UBTService_ClosestVisiblePlayer
{
	GENERATED_BODY()
	
protected:
	virtual void OnValueChanged(bool ValueSet, UBehaviorTreeComponent& OwnerComp) override;

};
