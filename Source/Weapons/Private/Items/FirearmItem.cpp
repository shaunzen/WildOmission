// Copyright Telephone Studios. All Rights Reserved.


#include "Items/FirearmItem.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Projectiles/FirearmProjectile.h"
#include "Camera/CameraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Log.h"

#include "DrawDebugHelpers.h"

AFirearmItem::AFirearmItem()
{
	MuzzleFlashEffect = nullptr;
	FireSound = nullptr;
	OutOfAmmoSound = nullptr;
	ProjectileClass = nullptr;
	AmmoItemID = NAME_None;

	Durability = 100;
	CurrentAmmo = 0;
	MaxAmmo = 0;

	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> MuzzleFlashEffectBlueprint(TEXT("/Game/Weapons/Art/NS_MuzzleFlash"));
	if (MuzzleFlashEffectBlueprint.Succeeded())
	{
		MuzzleFlashEffect = MuzzleFlashEffectBlueprint.Object;
	}
}

void AFirearmItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFirearmItem, CurrentAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFirearmItem, MaxAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFirearmItem, Durability, COND_OwnerOnly);
}

void AFirearmItem::Equip(APawn *InOwnerPawn, USkeletalMeshComponent *InThirdPersonMeshComponent, const FName &InItemName, const int8 &InFromSlotIndex, const uint32 &InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);
	
	RetrieveInventoryStats();
}

void AFirearmItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

	if (GetOwnerPawn()->IsLocallyControlled())
	{
		if (CurrentAmmo <= 0)
		{
			if (OutOfAmmoSound == nullptr)
			{
				return;
			}

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutOfAmmoSound, GetOwner()->GetActorLocation());
			return;
		}

		SpawnProjectile();
		SpawnMuzzleFlash();
		PlayFireSoundEffect();

		GetOwnerEquipComponent()->PlayMontage(PrimaryMontage, true);
		GetOwnerEquipComponent()->PlayItemMontage(PrimaryItemMontage, true);
	}

	if (HasAuthority())
	{
		if (CurrentAmmo <= 0)
		{
			return;
		}

		Multi_FireEffects();

		--CurrentAmmo;
		--Durability;
		if (Durability <= 0 && GetOwningPlayerInventory())
		{
			GetOwningPlayerInventory()->RemoveHeldItem();
		}
		UpdateInventoryStats();
	}
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

	if (GetOwnerPawn()->IsLocallyControlled())
	{
		GetOwnerEquipComponent()->PlayMontage(ReloadMontage, true);
		GetOwnerEquipComponent()->PlayItemMontage(ReloadItemMontage, true);
	}

	if (HasAuthority())
	{
		Multi_PlayThirdPersonReloadMontage();
	}
}

void AFirearmItem::OnReloadAnimationClimax(bool FromFirstPersonInstance)
{
	if (!HasAuthority())
	{
		return;
	}

	const int32 RemainingInventoryAmmo = GetRemainingAmmoInInventory();
	const int32 AmmoRequiredToFillMag = MaxAmmo - CurrentAmmo;
	
	int32 AmmoUsedInReload = 0;
	
	if (GetRemainingAmmoInInventory() < AmmoRequiredToFillMag)
	{
		AmmoUsedInReload += GetRemainingAmmoInInventory();
	}
	else
	{
		AmmoUsedInReload = AmmoRequiredToFillMag;
	}

	CurrentAmmo += AmmoUsedInReload;
	RemoveAmmoFromInventory(AmmoUsedInReload);
	UpdateInventoryStats();
}

void AFirearmItem::RetrieveInventoryStats()
{
	UPlayerInventoryComponent* OwnerInventory = GetOwningPlayerInventory();
	if (OwnerInventory == nullptr)
	{
		return;
	}

	FInventorySlot* FromSlot = OwnerInventory->GetSlot(this->GetFromSlotIndex());
	if (FromSlot == nullptr)
	{
		return;
	}

	CurrentAmmo = FromSlot->Item.GetStat(TEXT("CurrentAmmo"));
	MaxAmmo = FromSlot->Item.GetStat(TEXT("MaxAmmo"));
	Durability = FromSlot->Item.GetStat(TEXT("Durability"));
}

void AFirearmItem::UpdateInventoryStats()
{
	UPlayerInventoryComponent* OwnerInventory = GetOwningPlayerInventory();
	if (OwnerInventory == nullptr)
	{
		return;
	}

	FInventorySlot* FromSlot = OwnerInventory->GetSlot(this->GetFromSlotIndex(), true);
	if (FromSlot == nullptr)
	{
		return;
	}

	FromSlot->Item.SetStat(TEXT("CurrentAmmo"), CurrentAmmo);
	FromSlot->Item.SetStat(TEXT("Durability"), Durability);
	OwnerInventory->RequestInventoryRefresh();
}

void AFirearmItem::SpawnProjectile()
{
	if (GetOwnerPawn() == nullptr 
	|| GetOwner()->FindComponentByClass<UCameraComponent>() == nullptr
	|| GetOwnerEquipComponent() == nullptr
	|| ProjectileClass == nullptr)
	{
		return;
	}

	const FVector ProjectileSpawnLocation = GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetOwnerPawn();
	SpawnParams.Owner = GetOwnerPawn();
	
	GetWorld()->SpawnActor<AFirearmProjectile>(ProjectileClass, ProjectileSpawnLocation, GetOwnerEquipComponent()->GetOwnerControlRotation(), SpawnParams);
}

void AFirearmItem::SpawnMuzzleFlash()
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

void AFirearmItem::PlayThirdPersonFireAnimation()
{
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr || PrimaryMontage == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayMontage(PrimaryMontage, false);
	OwnerEquipComponent->PlayItemMontage(PrimaryItemMontage, false);
}

void AFirearmItem::PlayFireSoundEffect()
{
	if (FireSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetOwner()->GetActorLocation());
}

int32 AFirearmItem::GetRemainingAmmoInInventory() const
{
	if (GetOwner() == nullptr)
	{
		return 0;
	}

	UInventoryComponent* OwnerInventory = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventory == nullptr)
	{
		return 0;
	}

	return OwnerInventory->GetContents()->GetItemQuantity(AmmoItemID);
}

void AFirearmItem::RemoveAmmoFromInventory(const int32 AmountToRemove)
{
	if (GetOwner() == nullptr)
	{
		return;
	}

	UInventoryComponent* OwnerInventory = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventory == nullptr)
	{
		return;
	}

	FInventoryItem AmmoToRemove;
	AmmoToRemove.Name = AmmoItemID;
	AmmoToRemove.Quantity = AmountToRemove;
	OwnerInventory->RemoveItem(AmmoToRemove);
}

UPlayerInventoryComponent* AFirearmItem::GetOwningPlayerInventory() const
{
	if (GetOwner() == nullptr)
	{
		return nullptr;
	}

	return GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
}

void AFirearmItem::Multi_FireEffects_Implementation()
{
	if (GetOwnerPawn() == nullptr || GetOwnerPawn()->IsLocallyControlled() || (GetOwnerPawn()->IsLocallyControlled() && GetOwnerPawn()->GetRemoteRole() == ROLE_Authority))
	{
		return;
	}

	SpawnProjectile();
	SpawnMuzzleFlash();
	PlayThirdPersonFireAnimation();
	PlayFireSoundEffect();
}

void AFirearmItem::Multi_PlayThirdPersonReloadMontage_Implementation()
{
	if (GetOwnerPawn() == nullptr || GetOwnerPawn()->IsLocallyControlled())
	{
		return;
	}

	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayMontage(ReloadMontage, false);
	OwnerEquipComponent->PlayItemMontage(ReloadItemMontage, false);
}
