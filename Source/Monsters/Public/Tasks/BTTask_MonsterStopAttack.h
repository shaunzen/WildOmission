// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MonsterStopAttack.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API UBTTask_MonsterStopAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_MonsterStopAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
