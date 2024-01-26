// Copyright Telephone Studios. All Rights Reserved.


#include "Components/DistanceDespawnComponent.h"
#include "Components/ChunkInvokerComponent.h"

// Sets default values for this component's properties
UDistanceDespawnComponent::UDistanceDespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UDistanceDespawnComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	AActor* OwnerActor = GetOwner();
	if (World == nullptr || OwnerActor == nullptr || !OwnerActor->HasAuthority())
	{
		return;
	}

	FTimerHandle CheckDespawnConditionTimerHandle;
	FTimerDelegate CheckDespawnConditionTimerDelegate;
	CheckDespawnConditionTimerDelegate.BindUObject(this, &UDistanceDespawnComponent::CheckDespawnConditions);
	World->GetTimerManager().SetTimer(CheckDespawnConditionTimerHandle, CheckDespawnConditionTimerDelegate, 30.0f, true);
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