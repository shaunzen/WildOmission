// Copyright Telephone Studios. All Rights Reserved.


#include "TemporaryItemContainer.h"
#include "WildOmission/Components/ActorDespawnComponent.h"

ATemporaryItemContainer::ATemporaryItemContainer()
{
	DespawnComponent = CreateDefaultSubobject<UActorDespawnComponent>(FName("DespawnComponent"));
	DespawnComponent->SetDespawnTime(1200.0f);
}