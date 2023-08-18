// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ClosestVisiblePlayer.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API UBTService_ClosestVisiblePlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_ClosestVisiblePlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxChaseDistance;

};
