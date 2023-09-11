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
		GetOwnerEquipComponent()->PlayItemMontage(SecondaryMontage, SecondaryItemMontage);
	}
	if (GetOwnerPawn()->IsLocallyControlled())
	{
		GetOwnerEquipComponent()->PlayItemMontage(SecondaryMontage, SecondaryItemMontage);
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

		GetOwnerEquipComponent()->PlayItemMontage(PrimaryMontage, PrimaryItemMontage);
	}
	if (HasAuthority())
	{
		Multi_PlayFireEffects();

		--CurrentAmmo;
		--Durability;
		if (Durability <= 0 && GetOwningPlayerInventory())
		{
			GetOwningPlayerInventory()->RemoveHeldItem();
		}
		PushInventoryStats();
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

	OwnerEquipComponent->PlayItemMontage(SecondaryMontage, SecondaryItemMontage);
}
