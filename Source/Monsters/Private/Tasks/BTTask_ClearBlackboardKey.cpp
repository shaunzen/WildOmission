// Copyright Telephone Studios. All Rights Reserved.


#include "Tasks/BTTask_ClearBlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardKey::UBTTask_ClearBlackboardKey()
{
	NodeName = TEXT("Clear Blackboard Key");
}

EBTNodeResult::Type UBTTask_ClearBlackboardKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Type::Succeeded;
}
