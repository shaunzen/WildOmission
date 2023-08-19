// Copyright Telephone Studios. All Rights Reserved.


#include "Services/BTService_MonsterAttackRange.h"
#include "AIController.h"
#include "Monsters/Monster.h"

UBTService_MonsterAttackRange::UBTService_MonsterAttackRange()
{
	NodeName = TEXT("Calculate If Monster Is In Attack Range");
}

void UBTService_MonsterAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		return;
	}

	AMonster* OwnerMonster = Cast<AMonster>(OwnerController->GetPawn());
	if (OwnerMonster == nullptr)
	{
		return;
	}

	// check distance from attacking actor

	// set selected blackboard key

}
