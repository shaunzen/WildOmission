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
		UE_LOG(LogTemp, Warning, TEXT("ai owner null."));
		return;
	}

	AWildOmissionAICharacter* AICharacter = Cast<AWildOmissionAICharacter>(AIOwner->GetPawn());
	if (AICharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ai character null."));
		return;
	}

	UVitalsComponent* OwnerVitalsComponent = AICharacter->FindComponentByClass<UVitalsComponent>();
	if (OwnerVitalsComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("vitals null."));
		return;
	}

	const float CurrentHealth = OwnerVitalsComponent->GetHealth();
	const float LastHealth = BlackboardComponent->GetValueAsFloat(LastRememberedHealthKey.SelectedKeyName);
	if (LastHealth != 0.0f && CurrentHealth < LastHealth)
	{
		OnTakeAnyDamage(OwnerComp);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("LastHealth %f, Current Health %f, on Character %s"), LastHealth, CurrentHealth, *AICharacter->GetActorNameOrLabel());

	BlackboardComponent->SetValueAsFloat(LastRememberedHealthKey.SelectedKeyName, CurrentHealth);
}



void UBTService_HasRecentlyBeenDamaged::OnTakeAnyDamage(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Blackboard nullptr."));
		return;
	}
	
	OwnerBlackboard->SetValueAsBool(GetSelectedBlackboardKey(), true);
}
