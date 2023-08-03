// Copyright Telephone Studios. All Rights Reserved.


#include "Weapons/Firearm.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Projectiles/FirearmProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AFirearm::AFirearm()
{
	FireSound = nullptr;
	ProjectileClass = nullptr;
	AmmoItemID = NAME_None;
}

void AFirearm::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);

	RetrieveAmmoInformationFromInventory();
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

	if (ProjectileClass == nullptr)
	{
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


}

void AFirearm::RetrieveAmmoInformationFromInventory()
{
	UInventoryComponent* OwnerInventory = GetOwner()->FindComponentByClass<UInventoryComponent>();
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
}

void AFirearm::UpdateInventoryAmmoInformation()
{
	UInventoryComponent* OwnerInventory = GetOwner()->FindComponentByClass<UInventoryComponent>();
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
}

void AFirearm::Multi_PlayFireSound_Implementation()
{
	if (GetOwner() == nullptr || FireSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetOwner()->GetActorLocation());
}
