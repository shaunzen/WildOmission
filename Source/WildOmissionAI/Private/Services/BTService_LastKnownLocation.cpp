// Copyright Telephone Studios. All Rights Reserved.


#include "Services/BTService_LastKnownLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

UBTService_LastKnownLocation::UBTService_LastKnownLocation()
{
	NodeName = TEXT("Update Last Known Player Location");
}

void UBTService_LastKnownLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AActor* ChasingPlayerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(PlayerActorBlackboardKey.SelectedKeyName));
	if (ChasingPlayerActor == nullptr)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ChasingPlayerActor->GetActorLocation());
}
