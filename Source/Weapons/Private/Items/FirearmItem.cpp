// Copyright Telephone Studios. All Rights Reserved.


#include "Items/FirearmItem.h"
#include "Components/EquipComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"

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

}

void AFirearmItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnPrimaryAnimationClimax(FromFirstPersonInstance);

	Fire();
}

void AFirearmItem::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();

	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->StartAim();
}

void AFirearmItem::OnSecondaryReleased()
{
	Super::OnSecondaryReleased();

	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->StopAim();
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