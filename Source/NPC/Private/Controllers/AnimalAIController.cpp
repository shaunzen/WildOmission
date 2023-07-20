// Copyright Telephone Studios. All Rights Reserved.


#include "Controllers/AnimalAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Log.h"

AAnimalAIController::AAnimalAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AAnimalAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTreeComponent == nullptr)
	{
		UE_LOG(LogNPC, Warning, TEXT("BehaviorTreeComp nullptr"));
		return;
	}

	RunBehaviorTree(BehaviorTreeComponent->GetCurrentTree());
}
