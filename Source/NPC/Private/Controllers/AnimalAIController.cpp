// Copyright Telephone Studios. All Rights Reserved.


#include "Controllers/AnimalAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Log.h"

void AAnimalAIController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogNPC, Display, TEXT("AnimalAIController BeginPlay"));

	if (BehaviorTree == nullptr)
	{
		UE_LOG(LogNPC, Warning, TEXT("BehaviorTree nullptr"));
		return;
	}

	RunBehaviorTree(BehaviorTree);
	UE_LOG(LogNPC, Display, TEXT("Running Behavior Tree."));
}
