// Copyright Telephone Studios. All Rights Reserved.


#include "Items/TorchItem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ATorchItem::ATorchItem()
{
	PrimaryActorTick.bCanEverTick = false;

	FireParticleSystem = nullptr;
	BurningSound = nullptr;

	LightMontage = nullptr;
	PutOutMontage = nullptr;

	OnPose = nullptr;
	OffPose = nullptr;

	SpawnedFireParticles = nullptr;
	SpawnedBurningSound = nullptr;
	SpawnedLightComponent = nullptr;

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
	
	UAnimMontage* MontageToPlay = IsBurning ? PutOutMontage : LightMontage;
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (MontageToPlay == nullptr || OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayItemMontage(MontageToPlay);
}

void ATorchItem::OnSecondaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnSecondaryAnimationClimax(FromFirstPersonInstance);

	if (!HasAuthority())
	{
		return;
	}

	IsBurning = !IsBurning;
	OnRep_IsBurning();
}

void ATorchItem::OnUnequip()
{
	Super::OnUnequip();

	StopFireEffects();
}

FRotator ATorchItem::GetRightArmVelocityOffset() const
{
	return IsBurning ? RightArmVelocityOffset : FRotator::ZeroRotator;
}

void ATorchItem::OnRep_IsBurning()
{
	IsBurning ? StartFireEffects() : StopFireEffects();
}

void ATorchItem::DecrementDurability()
{
	UPlayerInventoryComponent* OwnerInventoryComponent = GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
	if (OwnerInventoryComponent == nullptr)
	{
		return;
	}

	FInventorySlot* FromSlot = OwnerInventoryComponent->GetSlot(GetFromSlotIndex());
	if (FromSlot == nullptr)
	{
		return;
	}

	int32 CurrentDurability = FromSlot->Item.GetStat(TEXT("Durability"));
	FromSlot->Item.SetStat(TEXT("Durability"), CurrentDurability - 1);

	if (FromSlot->Item.GetStat(TEXT("Durability")) <= 0)
	{
		OwnerInventoryComponent->RemoveHeldItem();
	}
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

	SpawnedLightComponent = NewObject<UPointLightComponent>(this, UPointLightComponent::StaticClass(), TEXT("PointLightComponent"));
	if (SpawnedLightComponent)
	{
		SpawnedLightComponent->RegisterComponent();
		SpawnedLightComponent->AttachToComponent(MeshComponentToAttachTo, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FireSocket"));
		
		SpawnedLightComponent->SetCastShadows(true);
		SpawnedLightComponent->SetIntensity(1000.0f);
		SpawnedLightComponent->SetSourceRadius(8.0f);
		SpawnedLightComponent->SetAttenuationRadius(4000.0f);
		SpawnedLightComponent->SetLightColor(FLinearColor(1.0f, 0.35f, 0.0f, 1.0f));
		SpawnedLightComponent->SetRelativeLocation(FVector(20.0f, -10.0f, 0.0f));
	}

	if (HasAuthority())
	{
		FTimerDelegate DecrementDurabilityTimerDelegate;
		DecrementDurabilityTimerDelegate.BindUObject(this, &ATorchItem::DecrementDurability);
		GetWorld()->GetTimerManager().SetTimer(DecrementDurabilityTimerHandle, DecrementDurabilityTimerDelegate, 1.0f, true);
	}

	EquipPose = OnPose;
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

	if (SpawnedLightComponent)
	{
		SpawnedLightComponent->DestroyComponent();
		SpawnedLightComponent = nullptr;
	}

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().ClearTimer(DecrementDurabilityTimerHandle);
	}

	EquipPose = OffPose;
}
