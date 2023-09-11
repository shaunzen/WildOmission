// Copyright Telephone Studios. All Rights Reserved.


#include "Items/FirearmItem.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Projectiles/WeaponProjectile.h"
#include "Camera/CameraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Log.h"

AFirearmItem::AFirearmItem()
{
	MuzzleFlashEffect = nullptr;

	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> MuzzleFlashEffectBlueprint(TEXT("/Game/Weapons/Art/NS_MuzzleFlash"));
	if (MuzzleFlashEffectBlueprint.Succeeded())
	{
		MuzzleFlashEffect = MuzzleFlashEffectBlueprint.Object;
	}
}

void AFirearmItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();
	
	if (!HasAmmo())
	{
		if (OutOfAmmoSound == nullptr)
		{
			return;
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutOfAmmoSound, GetOwner()->GetActorLocation());
		return;
	}

	GetOwnerEquipComponent()->PlayItemMontage(PrimaryMontage, PrimaryItemMontage);

}

void AFirearmItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnPrimaryAnimationClimax(FromFirstPersonInstance);

	Fire();
}

void AFirearmItem::OnReloadPressed()
{
	Super::OnReloadPressed();

	if (GetOwner() == nullptr 
		|| GetOwnerPawn() == nullptr 
		|| GetOwnerEquipComponent() == nullptr 
		|| CurrentAmmo == MaxAmmo
		|| GetRemainingAmmoInInventory() == 0
		|| GetOwnerEquipComponent()->IsMontagePlaying(ReloadMontage))
	{
		return;
	}

	GetOwnerEquipComponent()->PlayItemMontage(ReloadMontage, ReloadItemMontage);
}

void AFirearmItem::OnReloadAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnReloadAnimationClimax(FromFirstPersonInstance);

	Reload();
}

void AFirearmItem::PlayFireEffects()
{
	Super::PlayFireEffects();
	PlayMuzzleFlash();
}

void AFirearmItem::PlayMuzzleFlash()
{
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr || MuzzleFlashEffect == nullptr)
	{
		return;
	}

	const FRotator FirstPersonItemWorldRotation = OwnerEquipComponent->GetFirstPersonItemComponent()->GetComponentRotation();
	const FVector MuzzleLocation = GetOwnerPawn()->IsLocallyControlled()
		? OwnerEquipComponent->GetFirstPersonItemComponent()->GetComponentLocation() + FirstPersonItemWorldRotation.RotateVector(MuzzleComponent->GetRelativeLocation())
		: MuzzleComponent->GetComponentLocation();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashEffect, MuzzleLocation, GetOwnerEquipComponent()->GetOwnerControlRotation());
}

void AFirearmItem::Fire()
{
	PlayFireEffects();
	DecreaseAmmoAndDurability();
}