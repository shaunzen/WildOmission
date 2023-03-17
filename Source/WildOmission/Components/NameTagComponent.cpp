// Copyright Telephone Studios. All Rights Reserved.


#include "NameTagComponent.h"

void UNameTagComponent::BeginPlay()
{
	Super::BeginPlay();
	// Set text to owner player name
}

void UNameTagComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// set rotation to always face player
}