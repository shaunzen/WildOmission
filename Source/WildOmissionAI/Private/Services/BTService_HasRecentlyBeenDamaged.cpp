// Copyright Telephone Studios. All Rights Reserved.


#include "Services/BTService_HasRecentlyBeenDamaged.h"
#include "Components/VitalsComponent.h"
#include "Characters/WildOmissionAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "AIController.h"

UBTService_HasRecentlyBeenDamaged::UBTService_HasRecentlyBeenDamaged()
{
	NodeName = TEXT("Has Recently Been Damaged");
}

void UBTService_HasRecentlyBeenDamaged::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (BlackboardComponent == nullptr)
	{
		return;
	}

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (AIOwner == nullptr)
	{
		return;
	}

	APawn* OwnerPawn = AIOwner->GetPawn();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	UVitalsComponent* OwnerVitalsComponent = OwnerPawn->FindComponentByClass<UVitalsComponent>();
	if (OwnerVitalsComponent == nullptr)
	{
		return;
	}

	const float CurrentHealth = OwnerVitalsComponent->GetHealth();
	const float LastHealth = BlackboardComponent->GetValueAsFloat(LastRememberedHealthKey.SelectedKeyName);
	if (LastHealth != 0.0f && CurrentHealth < LastHealth)
	{
		OnTakeAnyDamage(OwnerComp);
	}
	
	BlackboardComponent->SetValueAsFloat(LastRememberedHealthKey.SelectedKeyName, CurrentHealth);
}



void UBTService_HasRecentlyBeenDamaged::OnTakeAnyDamage(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr)
	{
		return;
	}
	
	OwnerBlackboard->SetValueAsBool(GetSelectedBlackboardKey(), true);
}
