// Copyright Telephone Studios. All Rights Reserved.


#include "Items/TorchItem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/EquipComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ATorchItem::ATorchItem()
{
	PrimaryActorTick.bCanEverTick = false;

	FireParticleSystem = nullptr;
	BurningSound = nullptr;

	SpawnedFireParticles = nullptr;
	SpawnedBurningSound = nullptr;

	IsBurning = false;
}

void ATorchItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATorchItem, IsBurning);
}

void ATorchItem::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();

	if (!HasAuthority())
	{
		return;
	}

	IsBurning = !IsBurning;
	OnRep_IsBurning();
}

void ATorchItem::OnRep_IsBurning()
{
	IsBurning ? StartFireEffects() : StopFireEffects();
}

void ATorchItem::DecrementDurability()
{
	// TODO handle damage stuff here
}

USkeletalMeshComponent* ATorchItem::GetMeshComponentToAttachTo() const
{
	APawn* OwnerPawn = GetOwnerPawn();
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerPawn && OwnerEquipComponent && OwnerPawn->IsLocallyControlled() && OwnerPawn->GetController() && OwnerPawn->GetController()->IsPlayerController())
	{
		return OwnerEquipComponent->GetFirstPersonItemComponent();
	}

	return MeshComponent;
}

void ATorchItem::StartFireEffects()
{
	USkeletalMeshComponent* MeshComponentToAttachTo = GetMeshComponentToAttachTo();
	if (MeshComponentToAttachTo == nullptr)
	{
		return;
	}

	if (FireParticleSystem)
	{
		SpawnedFireParticles = UNiagaraFunctionLibrary::SpawnSystemAttached(FireParticleSystem, MeshComponentToAttachTo, TEXT("FireSocket"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	}

	if (BurningSound)
	{
		SpawnedBurningSound = UGameplayStatics::SpawnSoundAttached(BurningSound, MeshComponentToAttachTo, TEXT("FireSocket"), FVector::ZeroVector, EAttachLocation::Type::SnapToTarget, true);
	}

}

void ATorchItem::StopFireEffects()
{
	if (SpawnedFireParticles)
	{
		SpawnedFireParticles->DestroyComponent();
		SpawnedFireParticles = nullptr;
	}

	if (SpawnedBurningSound)
	{
		SpawnedBurningSound->DestroyComponent();
		SpawnedBurningSound = nullptr;
	}
}
