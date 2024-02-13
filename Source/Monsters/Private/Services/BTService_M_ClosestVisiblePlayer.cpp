// Copyright Telephone Studios. All Rights Reserved.


#include "Services/BTService_M_ClosestVisiblePlayer.h"
#include "AIController.h"
#include "Monsters/Monster.h"

void UBTService_M_ClosestVisiblePlayer::OnValueChanged(bool ValueSet, UBehaviorTreeComponent& OwnerComp)
{
	Super::OnValueChanged(ValueSet, OwnerComp);
	
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		return;
	}

	AMonster* OwnerMonster = Cast<AMonster>(OwnerController->GetPawn());
	if (OwnerMonster == nullptr)
	{
		return;
	}

	OwnerMonster->SetIdleSoundsEnabled(ValueSet);
}
