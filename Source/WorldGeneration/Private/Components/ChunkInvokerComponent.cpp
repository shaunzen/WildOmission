// Copyright Telephone Studios. All Rights Reserved.


#include "Components/ChunkInvokerComponent.h"
#include "Actors/Chunk.h"
#include "Log.h"

static TArray<UChunkInvokerComponent*> Invokers;
static const uint8 DEFAULT_RENDER_DISTANCE = 16;

// Sets default values for this component's properties
UChunkInvokerComponent::UChunkInvokerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	RenderDistance = DEFAULT_RENDER_DISTANCE;
}

// Called when the game starts
void UChunkInvokerComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr || !OwnerActor->HasAuthority())
	{
		return;
	}

	const int32 Index = Invokers.Add(this);

	UE_LOG(LogWorldGeneration, Verbose, TEXT("Adding Invoker %s at index %i. New Invoker count %i"), *OwnerActor->GetActorNameOrLabel(), Index, Invokers.Num());
}

void UChunkInvokerComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr || !OwnerActor->HasAuthority())
	{
		return;
	}

	Invokers.Remove(this);

	UE_LOG(LogWorldGeneration, Verbose, TEXT("Removing Invoker %s. New Invoker count %i"), *OwnerActor->GetActorNameOrLabel(), Invokers.Num());
}

void UChunkInvokerComponent::SetRenderDistance(const uint8& InRenderDistance)
{
	RenderDistance = InRenderDistance;
}

uint8 UChunkInvokerComponent::GetRenderDistance() const
{
	return RenderDistance;
}

float UChunkInvokerComponent::GetRenderDistanceCentimeters() const
{
	return RenderDistance * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale();
}

TArray<UChunkInvokerComponent*> UChunkInvokerComponent::GetAllInvokers()
{
	return Invokers;
}

uint8 UChunkInvokerComponent::GetDefaultRenderDistance()
{
	return DEFAULT_RENDER_DISTANCE;
}

float UChunkInvokerComponent::GetDefaultRenderDistanceCentimeters()
{
	return DEFAULT_RENDER_DISTANCE * AChunk::GetVertexSize() * AChunk::GetVertexDistanceScale();
}
