// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionSpectatorPawn.h"
#include "Components/ChunkInvokerComponent.h"

AWildOmissionSpectatorPawn::AWildOmissionSpectatorPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	ChunkInvokerComponent = CreateDefaultSubobject<UChunkInvokerComponent>(TEXT("ChunkInvokerComponent"));
	ChunkInvokerComponent->SetupAttachment(RootComponent);
}