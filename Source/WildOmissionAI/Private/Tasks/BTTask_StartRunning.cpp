// Copyright Telephone Studios. All Rights Reserved.


#include "Tasks/BTTask_StartRunning.h"
#include "Characters/WildOmissionAICharacter.h"
#include "AIController.h"

UBTTask_StartRunning::UBTTask_StartRunning()
{
	NodeName = TEXT("Start Running");

}

EBTNodeResult::Type UBTTask_StartRunning::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	OwnerCharacter->StartRunning();

	return EBTNodeResult::Type::Succeeded;
}
