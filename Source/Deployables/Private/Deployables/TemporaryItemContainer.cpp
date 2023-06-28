// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/TemporaryItemContainer.h"
#include "Components/ActorDespawnComponent.h"

ATemporaryItemContainer::ATemporaryItemContainer()
{
	DespawnComponent = CreateDefaultSubobject<UActorDespawnComponent>(FName("DespawnComponent"));
	DespawnComponent->SetDespawnTime(1200.0f);
}