// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "UBTService_ClosestPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API UUBTService_ClosestPlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
