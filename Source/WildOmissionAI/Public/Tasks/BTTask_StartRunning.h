// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StartRunning.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSIONAI_API UBTTask_StartRunning : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_StartRunning();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
