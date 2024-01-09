// Copyright Telephone Studios. All Rights Reserved.


#include "Services/BTService_ClosestVisiblePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_ClosestVisiblePlayer::UBTService_ClosestVisiblePlayer()
{
	NodeName = TEXT("Find Closest Chasable Player");
	MaxChaseDistance = 1500.0f;
}

void UBTService_ClosestVisiblePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
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
		if ((ClosestPlayerDistance != -1.0f && PlayerDistance > ClosestPlayerDistance) || PlayerDistance > MaxChaseDistance)
		{
			continue;
		}

		ClosestVisiblePlayer = VisiblePlayer;
	}

	// Set Blackboard value
	if (ClosestVisiblePlayer == nullptr)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ClosestVisiblePlayer->GetActorLocation());
}
