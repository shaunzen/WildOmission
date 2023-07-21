// Copyright Telephone Studios. All Rights Reserved.


#include "Controllers/AnimalController.h"

void AAnimalController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree == nullptr)
	{
		return;
	}

	RunBehaviorTree(BehaviorTree);
}
