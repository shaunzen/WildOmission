// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_MonsterAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API UBTService_MonsterAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_MonsterAttackRange();

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector AttackedActorBlackboardKey;
	
	UPROPERTY(EditAnywhere)
	float MaxAttackDistance;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
