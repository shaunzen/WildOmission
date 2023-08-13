// Copyright Telephone Studios. All Rights Reserved.


#include "Weapons/Firearm.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Projectiles/FirearmProjectile.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Log.h"

AFirearm::AFirearm()
{
	FireSound = nullptr;
	ProjectileClass = nullptr;
	AmmoItemID = NAME_None;

	Durability = 100;
	CurrentAmmo = 0;
	MaxAmmo = 0;
}

void AFirearm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFirearm, CurrentAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFirearm, MaxAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFirearm, Durability, COND_OwnerOnly);
}

void AFirearm::Equip(APawn *InOwnerPawn, USkeletalMeshComponent *InThirdPersonMeshComponent, const FName &InItemName, const int8 &InFromSlotIndex, const uint32 &InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);
	
	RetrieveInventoryStats();
}

void AFirearm::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

	if (CurrentAmmo == 0)
	{
		// TODO click sound
		return;
	}

	if (GetOwnerPawn()->IsLocallyControlled())
	{
		SimulateFire();
	}

	if (HasAuthority())
	{
		Fire();
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

void AFirearm::SimulateFire()
{
	if (!HasAuthority())
	{
		SpawnProjectile();
		PlayFireSoundEffect();
	}
	
	GetOwnerEquipComponent()->PlayItemMontage(PrimaryMontage, true);
}

void AFirearm::Fire()
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

void AFirearm::SpawnProjectile()
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

void AFirearm::PlayFireAnimation()
{
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr || PrimaryMontage == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayItemMontage(PrimaryMontage, false);
}

void AFirearm::PlayFireSoundEffect()
{
	if (FireSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetOwner()->GetActorLocation());
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

void AFirearm::Multi_FireEffects_Implementation()
{
	if (GetOwnerPawn() == nullptr || GetOwnerPawn()->IsLocallyControlled())
	{
		return;
	}

	SpawnProjectile();
	PlayFireAnimation();
	PlayFireSoundEffect();
}

void AFirearm::Multi_PlayThirdPersonReloadMontage_Implementation()
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

	OwnerEquipComponent->PlayItemMontage(ReloadMontage, false);
}
