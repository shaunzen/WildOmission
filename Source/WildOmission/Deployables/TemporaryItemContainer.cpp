// Copyright Telephone Studios. All Rights Reserved.


#include "TemporaryItemContainer.h"

ATemporaryItemContainer::ATemporaryItemContainer()
{
	DespawnTimeSeconds = 1200.0f;
}

void ATemporaryItemContainer::BeginPlay()
{
	Super::BeginPlay();
	
	DespawnTimeSeconds = 1200.0f;
}

void ATemporaryItemContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
	{
		return;
	}

	DespawnTimeSeconds -= DeltaTime;

	if (DespawnTimeSeconds < KINDA_SMALL_NUMBER)
	{
		Destroy();
	}
}