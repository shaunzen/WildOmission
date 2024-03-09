// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Sapling.h"
#include "ChunkManager.h"
#include "Actors/Chunk.h"
#include "Structs/BiomeGenerationData.h"
#include "UObject/ConstructorHelpers.h"

ASapling::ASapling()
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
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	
	// Get Biome
	const FBiomeGenerationData* Biome = AChunk::GetBiomeAtLocation(FVector2D(this->GetActorLocation().X, this->GetActorLocation().Y));
	if (Biome == nullptr || Biome->Trees.Spawnables.IsEmpty())
	{
		return;
	}

	// Decide which tree we will become
	const int32 MatureStateIndex = FMath::RandRange(0, Biome->Trees.Spawnables.Num() - 1);

	// Spawn it
	AActor* MaturePlant = World->SpawnActor<AActor>(Biome->Trees.Spawnables[MatureStateIndex].BlueprintClass, this->GetActorLocation(), this->GetActorRotation());
	if (MaturePlant == nullptr)
	{
		return;
	}

	AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
	if (ChunkManager == nullptr)
	{
		return;
	}

	MaturePlant->AttachToActor(ChunkManager->GetChunkAtLocation(this->GetActorLocation()), FAttachmentTransformRules::KeepWorldTransform);

	// Destroy us
	Multi_PlayDestructionEffects();
	this->Destroy();
}