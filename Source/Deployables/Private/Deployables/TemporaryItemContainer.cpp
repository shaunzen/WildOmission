// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/TemporaryItemContainer.h"
#include "Components/TimerDespawnComponent.h"

ATemporaryItemContainer::ATemporaryItemContainer()
{
	DespawnComponent = CreateDefaultSubobject<UTimerDespawnComponent>(TEXT("DespawnComponent"));
	DespawnComponent->SetDespawnTime(1200.0f);
}