// Copyright Telephone Studios. All Rights Reserved.


#include "Components/DistanceDespawnComponent.h"
#include "Components/ChunkInvokerComponent.h"

const static float CONDITION_CHECK_INTERVAL = 10.0f;

// Sets default values for this component's properties
UDistanceDespawnComponent::UDistanceDespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConditionCheckCounter = CONDITION_CHECK_INTERVAL;
}

void UDistanceDespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr || !OwnerActor->HasAuthority())
	{
		return;
	}

	ConditionCheckCounter += DeltaTime;

	if (ConditionCheckCounter < CONDITION_CHECK_INTERVAL)
	{
		return;
	}
	
	CheckDespawnConditions();
}

void UDistanceDespawnComponent::CheckDespawnConditions()
{
	bool ShouldDespawn = true;
	TArray<UChunkInvokerComponent*> ChunkInvokers = UChunkInvokerComponent::GetAllInvokers();
	for (UChunkInvokerComponent* ChunkInvoker : ChunkInvokers)
	{
		if (ChunkInvoker == nullptr)
		{
			continue;
		}
		
		const FVector FlattenedComponentLocation(this->GetComponentLocation().X, this->GetComponentLocation().Y, 0.0f);
		const FVector FlattenedInvokerLocation(ChunkInvoker->GetComponentLocation().X, ChunkInvoker->GetComponentLocation().Y, 0.0f);

		const float DistanceFromInvoker = FVector::Distance(FlattenedInvokerLocation, FlattenedComponentLocation);
		if (DistanceFromInvoker >= ChunkInvoker->GetRenderDistanceCentimeters())
		{
			continue;
		}

		ShouldDespawn = false;
		break;
	}

	if (ShouldDespawn && OnDespawnConditionMet.IsBound())
	{
		OnDespawnConditionMet.Broadcast();
	}
}