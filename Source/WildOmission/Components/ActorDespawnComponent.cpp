// Copyright Telephone Studios. All Rights Reserved.


#include "ActorDespawnComponent.h"

// Sets default values for this component's properties
UActorDespawnComponent::UActorDespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DespawnTimeSeconds = 500.0f;
}

// Called every frame
void UActorDespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	DespawnTimeSeconds -= DeltaTime;

	if (DespawnTimeSeconds < KINDA_SMALL_NUMBER)
	{
		GetOwner()->Destroy();
	}
}

float UActorDespawnComponent::GetTimeRemaining() const
{
	return DespawnTimeSeconds;
}

void UActorDespawnComponent::SetDespawnTime(float TimeInSeconds)
{
	DespawnTimeSeconds = TimeInSeconds;
}