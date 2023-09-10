// Copyright Telephone Studios. All Rights Reserved.


#include "Items/BowItem.h"
#include "Components/EquipComponent.h"
#include "Components/PlayerInventoryComponent.h"

ABowItem::ABowItem()
{
	ProjectileClass = nullptr;
	AtFullDraw = false;
	DefaultEquipPose = nullptr;
	DrawnEquipPose = nullptr;
	EquipPose = nullptr;
}

void ABowItem::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);

	AtFullDraw = false;
	EquipPose = DefaultEquipPose;
}

void ABowItem::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();

	if (GetOwnerPawn() && GetOwnerPawn()->IsLocallyControlled())
	{
		GetOwnerEquipComponent()->PlayItemMontage(SecondaryMontage, true);
		GetOwnerEquipComponent()->PlayItemMontage(SecondaryItemMontage, true);
	}
	if (GetOwnerPawn()->IsLocallyControlled())
	{
		GetOwnerEquipComponent()->PlayMontage(SecondaryMontage, true);
		GetOwnerEquipComponent()->PlayItemMontage(SecondaryItemMontage, true);
	}

	if (HasAuthority())
	{
		Multi_PlayThirdPersonSecondaryMontage();
	}
}

void ABowItem::OnSecondaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnSecondaryAnimationClimax(FromFirstPersonInstance);
	
	AtFullDraw = true;
	EquipPose = DrawnEquipPose;
	UE_LOG(LogTemp, Warning, TEXT("Secondary Climax."));
}

void ABowItem::OnSecondaryReleased()
{
	Super::OnSecondaryReleased();
	
	if (!AtFullDraw)
	{
		return;
	}

	if (GetOwnerPawn() && GetOwnerPawn()->IsLocallyControlled())
	{
		SpawnProjectile();
		PlayFireSoundEffect();

		GetOwnerEquipComponent()->PlayMontage(PrimaryMontage, true);
		GetOwnerEquipComponent()->PlayItemMontage(PrimaryItemMontage, true);
	}
	if (HasAuthority())
	{
		Multi_FireEffects();

		--CurrentAmmo;
		--Durability;
		if (Durability <= 0 && GetOwningPlayerInventory())
		{
			GetOwningPlayerInventory()->RemoveHeldItem();
		}
		UpdateInventoryStats();
	}
	EquipPose = DefaultEquipPose;
	AtFullDraw = false;
}

void ABowItem::Multi_PlayThirdPersonSecondaryMontage_Implementation()
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

	OwnerEquipComponent->PlayMontage(SecondaryMontage, false);
	OwnerEquipComponent->PlayItemMontage(SecondaryItemMontage, false);
}
