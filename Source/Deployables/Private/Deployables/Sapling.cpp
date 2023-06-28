// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Sapling.h"
#include "Actors/HarvestableResource.h"

ASapling::ASapling()
{
	MinMatureTimeSeconds = 15.0f;
	MaxMatureTimeSeconds = 1200.0f;
	TimeToMatureSeconds = 0.0f;
}

void ASapling::OnSpawn()
{
	Super::OnSpawn();

	TimeToMatureSeconds = FMath::RandRange(MinMatureTimeSeconds, MaxMatureTimeSeconds);
}

void ASapling::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
	{
		return;
	}

	TimeToMatureSeconds -= DeltaTime;

	if (TimeToMatureSeconds < 0.0f)
	{
		GrowUp();
	}
}

void ASapling::GrowUp()
{
	// Decide which tree we will become
	int32 MatureStateIndex = FMath::RandRange(0, MatureStates.Num() - 1);

	// Check that tree is valid
	if (MatureStates[MatureStateIndex] == nullptr)
	{
		return;
	}

	// Spawn it
	GetWorld()->SpawnActor<AActor>(MatureStates[MatureStateIndex], this->GetActorLocation(), this->GetActorRotation());
	
	// Destroy us
	this->Destroy();
}