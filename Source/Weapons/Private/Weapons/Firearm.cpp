// Copyright Telephone Studios. All Rights Reserved.


#include "Weapons/Firearm.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Projectiles/FirearmProjectile.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AFirearm::AFirearm()
{
	FireSound = nullptr;
	ProjectileClass = nullptr;
	AmmoItemID = NAME_None;

	Durability = 100;
	CurrentAmmo = 0;
	MaxAmmo = 0;
}

void AFirearm::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);

	RetrieveInventoryStats();
}

void AFirearm::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

	if (GetOwnerPawn()->IsLocallyControlled())
	{
		GetOwnerEquipComponent()->PlayItemMontage(PrimaryMontage, true);
	}

	if (HasAuthority())
	{
		FireProjectile();
	}
}

void AFirearm::OnReloadPressed()
{
	Super::OnReloadPressed();

	if (GetOwner() == nullptr 
		|| GetOwnerPawn() == nullptr 
		|| GetOwnerEquipComponent() == nullptr 
		|| CurrentAmmo == MaxAmmo
		|| GetRemainingAmmoInInventory() == 0
		|| GetOwnerEquipComponent()->IsItemMontagePlaying(ReloadMontage))
	{
		return;
	}

	if (GetOwnerPawn()->IsLocallyControlled())
	{
		GetOwnerEquipComponent()->PlayItemMontage(ReloadMontage, true);
	}

	if (HasAuthority())
	{
		Multi_PlayThirdPersonReloadMontage();
	}
}

void AFirearm::OnReloadAnimationClimax(bool FromFirstPersonInstance)
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

void AFirearm::FireProjectile()
{
	if (ProjectileClass == nullptr 
		|| CurrentAmmo <= 0
		|| GetOwner() == nullptr
		|| GetOwner()->FindComponentByClass<UCameraComponent>() == nullptr)
	{
		// TODO play click
		UE_LOG(LogTemp, Warning, TEXT("Out of ammo"));
		return;
	}

	Multi_PlayFireEffects();

	// Find Spawn location for projectile
	const FVector ProjectileSpawnLocation = GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	
	// Spawn the projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetOwnerPawn();
	SpawnParams.Owner = GetOwner();
	GetWorld()->SpawnActor<AFirearmProjectile>(ProjectileClass, ProjectileSpawnLocation, GetOwnerEquipComponent()->GetOwnerControlRotation(), SpawnParams);

	--CurrentAmmo;
	--Durability;
	if (Durability <= 0 && GetOwningPlayerInventory())
	{
		GetOwningPlayerInventory()->RemoveHeldItem();
	}
	UpdateInventoryStats();
}

void AFirearm::RetrieveInventoryStats()
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

void AFirearm::UpdateInventoryStats()
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

int32 AFirearm::GetRemainingAmmoInInventory() const
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

void AFirearm::RemoveAmmoFromInventory(const int32 AmountToRemove)
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

UPlayerInventoryComponent* AFirearm::GetOwningPlayerInventory() const
{
	if (GetOwner() == nullptr)
	{
		return nullptr;
	}

	return GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
}

void AFirearm::Multi_PlayThirdPersonReloadMontage_Implementation()
{
	if (GetOwnerPawn() == nullptr || GetOwnerPawn()->IsLocallyControlled())
	{
		return;
	}

	UEquipComponent* OwnerEquipComponent = GetOwner()->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayItemMontage(ReloadMontage, false);
}

void AFirearm::Multi_PlayFireEffects_Implementation()
{
	if (GetOwner() == nullptr)
	{
		return;
	}

	if (PrimaryMontage)
	{
		GetOwnerEquipComponent()->PlayItemMontage(PrimaryMontage, false);
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetOwner()->GetActorLocation());
	}
}
