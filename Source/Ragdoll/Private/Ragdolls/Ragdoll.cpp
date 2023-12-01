// Copyright Telephone Studios. All Rights Reserved.


#include "Ragdolls/Ragdoll.h"
#include "Components/TimerDespawnComponent.h"
#include "Components/HarvestableComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Net/UnrealNetwork.h"

ARagdoll::ARagdoll()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
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

void ARagdoll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MeshComponent == nullptr)
	{
		return;
	}

	APhysicsVolume* MeshPhysicsVolume = MeshComponent->GetPhysicsVolume();
	if (MeshPhysicsVolume == nullptr)
	{
		return;
	}

	
	if (MeshPhysicsVolume->bWaterVolume == true)
	{
		const FVector ForceVector = FVector::UpVector * 100.0f * DeltaTime;
		MeshComponent->AddForce(ForceVector);
	}
}

FName ARagdoll::GetIdentifier() const
{
	return Identifier;
}

USkeletalMeshComponent* ARagdoll::GetMeshComponent() const
{
	return MeshComponent;
}