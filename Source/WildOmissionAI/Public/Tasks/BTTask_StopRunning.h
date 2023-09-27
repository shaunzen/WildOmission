// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopRunning.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSIONAI_API UBTTask_StopRunning : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_StopRunning();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
