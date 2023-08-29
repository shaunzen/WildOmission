// Copyright Telephone Studios. All Rights Reserved.


#include "Characters/WildOmissionAIRagdoll.h"
#include "Components/TimerDespawnComponent.h"
#include "Components/HarvestableComponent.h"
#include "Net/UnrealNetwork.h"

AWildOmissionAIRagdoll::AWildOmissionAIRagdoll()
{
	bReplicates = true;
	SetReplicateMovement(true);
	NetUpdateFrequency = 1.0f;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	RootComponent = MeshComponent;

	DespawnComponent = CreateDefaultSubobject<UTimerDespawnComponent>(TEXT("DespawnComponent"));
	DespawnComponent->SetDespawnTime(1200.0f);

	HarvestableComponent = CreateDefaultSubobject<UHarvestableComponent>(TEXT("HarvestableComponent"));
}

FName AWildOmissionAIRagdoll::GetIdentifier() const
{
	return Identifier;
}
