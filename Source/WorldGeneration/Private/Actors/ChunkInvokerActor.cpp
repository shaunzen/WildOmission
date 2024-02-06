// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/ChunkInvokerActor.h"
#include "Components/ChunkInvokerComponent.h"

// Sets default values
AChunkInvokerActor::AChunkInvokerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ChunkInvokerComponent = CreateDefaultSubobject<UChunkInvokerComponent>(TEXT("ChunkInvokerComponent"));
	ChunkInvokerComponent->SetRenderDistance(4);

}

void AChunkInvokerActor::SetRenderDistance(const uint8& InRenderDistance)
{
	if (ChunkInvokerComponent == nullptr)
	{
		return;
	}

	ChunkInvokerComponent->SetRenderDistance(InRenderDistance);
}

uint8 AChunkInvokerActor::GetRenderDistance() const
{
	if (ChunkInvokerComponent == nullptr)
	{
		return 1;
	}

	return ChunkInvokerComponent->GetRenderDistance();
}

