// Copyright Telephone Studios. All Rights Reserved.


#include "Weapons/Firearm.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Projectiles/FirearmProjectile.h"
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

	FireProjectile();
}

void AFirearm::OnReloadPressed()
{
	Super::OnReloadPressed();

}

void AFirearm::FireProjectile()
{
	Multi_PlayFireSound();

	if (ProjectileClass == nullptr || CurrentAmmo <= 0)
	{
		// TODO play click
		UE_LOG(LogTemp, Warning, TEXT("Out of ammo"));
		return;
	}

	// Find Spawn location for projectile
	const FVector OwnerForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetOwnerEquipComponent()->GetOwnerControlRotation().Yaw, 0.0f));
	const FVector ProjectileSpawnLocation = GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, 70.0f) + OwnerForwardVector * 50.0f;
	
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

UPlayerInventoryComponent* AFirearm::GetOwningPlayerInventory() const
{
	if (GetOwner() == nullptr)
	{
		return nullptr;
	}

	return GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
}

void AFirearm::Multi_PlayFireSound_Implementation()
{
	if (GetOwner() == nullptr || FireSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetOwner()->GetActorLocation());
}
