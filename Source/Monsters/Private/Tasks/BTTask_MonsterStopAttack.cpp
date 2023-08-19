// Copyright Telephone Studios. All Rights Reserved.


#include "Tasks/BTTask_MonsterStopAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monsters/Monster.h"
#include "AIController.h"

UBTTask_MonsterStopAttack::UBTTask_MonsterStopAttack()
{
	NodeName = TEXT("Monster Stop Attack");
}

EBTNodeResult::Type UBTTask_MonsterStopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}
	AMonster* OwnerMonster = Cast<AMonster>(OwnerController->GetPawn());
	if (OwnerMonster == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}

	UObject* BlackboardObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey());
	AActor* BlackboardActor = Cast<AActor>(BlackboardObject);
	if (BlackboardActor == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}

	OwnerMonster->StopAttack();

	return EBTNodeResult::Type::Succeeded;
}
