// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Seedling.h"
#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "UObject/ConstructorHelpers.h"

ASeedling::ASeedling()
{
	PrimaryActorTick.bCanEverTick = true;

	MinMatureTimeSeconds = 15.0f;
	MaxMatureTimeSeconds = 1200.0f;
	TimeToMatureSeconds = 0.0f;

	static ConstructorHelpers::FObjectFinder<USoundBase> SeedlingGrowUpSound(TEXT("/Game/Deployables/Audio/A_Thud"));
	if (SeedlingGrowUpSound.Succeeded())
	{
		DestructionSound = SeedlingGrowUpSound.Object;
	}
}

void ASeedling::OnSpawn()
{
	Super::OnSpawn();

	TimeToMatureSeconds = FMath::RandRange(MinMatureTimeSeconds, MaxMatureTimeSeconds);
}

void ASeedling::Tick(float DeltaTime)
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

void ASeedling::GrowUp()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	// Decide which tree we will become
	int32 MatureStateIndex = FMath::RandRange(0, MatureStates.Num() - 1);

	// Check that tree is valid
	if (MatureStates[MatureStateIndex] == nullptr)
	{
		return;
	}

	// Spawn it
	AActor* MaturePlant = World->SpawnActor<AActor>(MatureStates[MatureStateIndex], this->GetActorLocation(), this->GetActorRotation());
	if (MaturePlant == nullptr)
	{
		return;
	}

	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	MaturePlant->AttachToActor(ChunkManager->GetChunkAtLocation(this->GetActorLocation());

	// Destroy us
	Multi_PlayDestructionEffects();
	this->Destroy();
}