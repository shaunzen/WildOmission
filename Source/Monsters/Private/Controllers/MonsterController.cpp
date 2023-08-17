// Copyright Telephone Studios. All Rights Reserved.


#include "Controllers/MonsterController.h"

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() || BehaviorTree == nullptr)
	{
		return;
	}

	RunBehaviorTree(BehaviorTree);
}
