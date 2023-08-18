// Copyright Telephone Studios. All Rights Reserved.


#include "Services/UBTService_ClosestPlayerLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UUBTService_ClosestPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Get AI Controller and Pawn
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		return;
	}
	APawn* OwnerPawn = OwnerController->GetPawn();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	// Create a list of all visible players from this AI
	TArray<APawn*> VisiblePlayers;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController == nullptr)
		{
			continue;
		}

		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn == nullptr)
		{
			continue;
		}

		if (!OwnerController->LineOfSightTo(PlayerPawn))
		{
			continue;
		}

		VisiblePlayers.Add(PlayerPawn);
	}

	// If no visible players found, return
	if (VisiblePlayers.IsEmpty())
	{
		return;
	}

	// Find the closest visible player
	APawn* ClosestVisiblePlayer = nullptr;
	float ClosestPlayerDistance = -1.0f;
	for (APawn* VisiblePlayer : VisiblePlayers)
	{
		if (VisiblePlayer == nullptr)
		{
			continue;
		}

		const float PlayerDistance = FVector::Distance(OwnerPawn->GetActorLocation(), VisiblePlayer->GetActorLocation());
		if (ClosestPlayerDistance != -1.0f && PlayerDistance > ClosestPlayerDistance)
		{
			continue;
		}

		ClosestVisiblePlayer = VisiblePlayer;
	}

	// Set Blackboard value
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ClosestVisiblePlayer->GetActorLocation());
}
