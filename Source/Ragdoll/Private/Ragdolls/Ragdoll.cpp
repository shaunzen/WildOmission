// Copyright Telephone Studios. All Rights Reserved.


#include "Ragdolls/Ragdoll.h"
#include "Components/TimerDespawnComponent.h"
#include "Components/HarvestableComponent.h"
#include "Net/UnrealNetwork.h"

ARagdoll::ARagdoll()
{
	bReplicates = true;
	NetUpdateFrequency = 2.0f;
	
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
	DespawnComponent->SetDespawnTime(600.0f);

	HarvestableComponent = CreateDefaultSubobject<UHarvestableComponent>(TEXT("HarvestableComponent"));
}

FName ARagdoll::GetIdentifier() const
{
	return Identifier;
}

USkeletalMeshComponent* ARagdoll::GetMeshComponent() const
{
	return MeshComponent;
}