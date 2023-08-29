// Copyright Telephone Studios. All Rights Reserved.


#include "Controllers/WildOmissionAIController.h"

void AWildOmissionAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() || BehaviorTree == nullptr)
	{
		return;
	}

	RunBehaviorTree(BehaviorTree);
}
