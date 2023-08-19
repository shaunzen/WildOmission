// Copyright Telephone Studios. All Rights Reserved.


#include "Services/BTService_MonsterAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Monsters/Monster.h"
#include "AIController.h"

UBTService_MonsterAttackRange::UBTService_MonsterAttackRange()
{
	NodeName = TEXT("Calculate If Monster Is In Attack Range");
	MaxAttackDistance = 300.0f;
}

void UBTService_MonsterAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		return;
	}

	AMonster* OwnerMonster = Cast<AMonster>(OwnerController->GetPawn());
	if (OwnerMonster == nullptr)
	{
		return;
	}

	// Check distance from attacked actor
	AActor* AttackedActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackedActorBlackboardKey.SelectedKeyName));
	if (AttackedActor == nullptr)
	{
		return;
	}

	// Set selected blackboard key respectively
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), FVector::Distance(OwnerMonster->GetActorLocation(), AttackedActor->GetActorLocation()) < MaxAttackDistance);
}
