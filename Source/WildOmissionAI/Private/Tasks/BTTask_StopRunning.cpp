// Copyright Telephone Studios. All Rights Reserved.


#include "Tasks/BTTask_StopRunning.h"
#include "Characters/WildOmissionAICharacter.h"
#include "AIController.h"

UBTTask_StopRunning::UBTTask_StopRunning()
{
	NodeName = TEXT("Stop Running");
}

EBTNodeResult::Type UBTTask_StopRunning::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (AIOwner == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}

	AWildOmissionAICharacter* OwnerCharacter = Cast<AWildOmissionAICharacter>(AIOwner->GetPawn());
	if (OwnerCharacter == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}

	OwnerCharacter->StopRunning();

	return EBTNodeResult::Type::Succeeded;
}
