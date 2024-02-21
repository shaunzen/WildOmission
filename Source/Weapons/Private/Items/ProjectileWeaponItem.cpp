// Copyright Telephone Studios. All Rights Reserved.


#include "Items/ProjectileWeaponItem.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Projectiles/WeaponProjectile.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
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

	DOREPLIFETIME(AProjectileWeaponItem, CurrentAmmo);
	DOREPLIFETIME_CONDITION(AProjectileWeaponItem, MaxAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AProjectileWeaponItem, Durability, COND_OwnerOnly);
}

void AProjectileWeaponItem::Equip(APawn *InOwnerPawn, USkeletalMeshComponent *InThirdPersonMeshComponent, const FName &InItemName, const int8 &InFromSlotIndex, const uint32 &InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);
	
	PullInventoryStats();
}

bool AProjectileWeaponItem::HasAmmo() const
{
	if (CurrentAmmo <= 0)
	{
		return false;
	}

	return true;
}

void AProjectileWeaponItem::PullInventoryStats()
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

void AProjectileWeaponItem::PushInventoryStats()
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
	FRotator ProjectileSpawnRotation = GetOwnerEquipComponent()->GetOwnerControlRotation();

	// Calculate an offset based on player velocity
	FVector PlayerVelocity = GetOwnerPawn()->GetVelocity();
	PlayerVelocity.Z = 0.0f;
	const float NormalizedPlayerVelocity = PlayerVelocity.Length() / 600.0f;
	const FVector2D SpawnRotationOffset = FMath::RandPointInCircle(NormalizedPlayerVelocity * 10.0f);

	// Apply that offset to the SpawnRotation
	ProjectileSpawnRotation += FRotator(SpawnRotationOffset.Y, SpawnRotationOffset.X, 0.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetOwnerPawn();
	SpawnParams.Owner = GetOwnerPawn();
	
	AWeaponProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AWeaponProjectile>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation, SpawnParams);
	if (SpawnedProjectile == nullptr)
	{
		return;
	}

	SpawnedProjectile->Setup(AmmoItemID);
}

void AProjectileWeaponItem::PlayFireSoundEffect()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	const FVector SoundLocation = OwnerActor->GetActorLocation() + (OwnerActor->GetActorForwardVector() * 100.0f);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, SoundLocation);
}

void AProjectileWeaponItem::Reload()
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
	PushInventoryStats();
}

void AProjectileWeaponItem::DecreaseAmmoAndDurability()
{
	if (!HasAuthority())
	{
		return;
	}

	--CurrentAmmo;
	--Durability;
	
	if (Durability <= 0 && GetOwningPlayerInventory())
	{
		GetOwningPlayerInventory()->Multi_PlayItemBreakSound();
		GetOwningPlayerInventory()->RemoveHeldItem();
	}

	PushInventoryStats();
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

void AProjectileWeaponItem::PlayFireEffects()
{
	if (HasAuthority())
	{
		SpawnProjectile();
	}

	PlayFireSoundEffect();
}
