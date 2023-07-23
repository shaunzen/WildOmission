// Copyright Telephone Studios. All Rights Reserved.


#include "Tasks/BTTask_GetRandomLocation.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Log.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation()
{
	NodeName = TEXT("Find Random Reachable Location");
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FVector CurrentLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavigationSystem == nullptr)
	{
		UE_LOG(LogAnimals, Warning, TEXT("GetRandomLocation, Failed to find NavigationSystem in world."));
		return EBTNodeResult::Type::Aborted;
	}
	FNavLocation NavLocation;
	if (!NavigationSystem->GetRandomReachablePointInRadius(CurrentLocation, 500.0f, NavLocation))
	{
		UE_LOG(LogAnimals, Warning, TEXT("GetRandomLocation, Failed to find random location."));
		return EBTNodeResult::Type::Aborted;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
	
	return EBTNodeResult::Type::Succeeded;
}
