// Copyright Telephone Studios. All Rights Reserved.


#include "Components/TimerDespawnComponent.h"

// Sets default values for this component's properties
UTimerDespawnComponent::UTimerDespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DespawnTimeSeconds = 500.0f;
}

// Called every frame
void UTimerDespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

float UTimerDespawnComponent::GetTimeRemaining() const
{
	return DespawnTimeSeconds;
}

void UTimerDespawnComponent::SetDespawnTime(float TimeInSeconds)
{
	DespawnTimeSeconds = TimeInSeconds;
}