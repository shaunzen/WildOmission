// Copyright Telephone Studios. All Rights Reserved.


#include "Items/ProjectileWeaponItem.h"
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

AProjectileWeaponItem::AProjectileWeaponItem()
{
	FireSound = nullptr;
	OutOfAmmoSound = nullptr;
	ProjectileClass = nullptr;
	AmmoItemID = NAME_None;

	Durability = 100;
	CurrentAmmo = 0;
	MaxAmmo = 0;
}

void AProjectileWeaponItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AProjectileWeaponItem, CurrentAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AProjectileWeaponItem, MaxAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AProjectileWeaponItem, Durability, COND_OwnerOnly);
}

void AProjectileWeaponItem::Equip(APawn *InOwnerPawn, USkeletalMeshComponent *InThirdPersonMeshComponent, const FName &InItemName, const int8 &InFromSlotIndex, const uint32 &InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);
	
	RetrieveInventoryStats();
}

bool AProjectileWeaponItem::HasAmmo() const
{
	if (CurrentAmmo <= 0)
	{
		return false;
	}

	return true;
}

bool AProjectileWeaponItem::CanFire() const
{
	return true;
}

void AProjectileWeaponItem::RetrieveInventoryStats()
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

void AProjectileWeaponItem::UpdateInventoryStats()
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

void AProjectileWeaponItem::SpawnProjectile()
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
	
	GetWorld()->SpawnActor<AWeaponProjectile>(ProjectileClass, ProjectileSpawnLocation, GetOwnerEquipComponent()->GetOwnerControlRotation(), SpawnParams);
}

void AProjectileWeaponItem::PlayThirdPersonFireAnimation()
{
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr || PrimaryMontage == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayMontage(PrimaryMontage);
	OwnerEquipComponent->PlayItemMontage(PrimaryItemMontage);
}

void AProjectileWeaponItem::PlayFireSoundEffect()
{
	if (FireSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetOwner()->GetActorLocation());
}

int32 AProjectileWeaponItem::GetRemainingAmmoInInventory() const
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

void AProjectileWeaponItem::RemoveAmmoFromInventory(const int32 AmountToRemove)
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

UPlayerInventoryComponent* AProjectileWeaponItem::GetOwningPlayerInventory() const
{
	if (GetOwner() == nullptr)
	{
		return nullptr;
	}

	return GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
}

void AProjectileWeaponItem::Multi_FireEffects_Implementation()
{
	if (GetOwnerPawn() == nullptr || GetOwnerPawn()->IsLocallyControlled() || (GetOwnerPawn()->IsLocallyControlled() && GetOwnerPawn()->GetRemoteRole() == ROLE_Authority))
	{
		return;
	}

	PlayFireEffects();
}

void AProjectileWeaponItem::PlayFireEffects()
{
	SpawnProjectile();
	PlayThirdPersonFireAnimation();
	PlayFireSoundEffect();
}
