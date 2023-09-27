// Copyright Telephone Studios. All Rights Reserved.


#include "Tasks/BTTask_GetRandomLocation.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Log.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation()
{
	NodeName = TEXT("Find Random Reachable Location");
	Radius = 5000.0f;
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FVector CurrentLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavigationSystem == nullptr)
	{
		UE_LOG(LogWildOmissionAI, Warning, TEXT("BTTask_GetRandomLocation, Failed to find NavigationSystem in world."));
		return EBTNodeResult::Type::Failed;
	}
	FNavLocation NavLocation;
	if (!NavigationSystem->GetRandomReachablePointInRadius(CurrentLocation, Radius, NavLocation))
	{
		UE_LOG(LogWildOmissionAI, Verbose, TEXT("BTTask_GetRandomLocation, Failed to find random location."));
		return EBTNodeResult::Type::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
	
	return EBTNodeResult::Type::Succeeded;
}
