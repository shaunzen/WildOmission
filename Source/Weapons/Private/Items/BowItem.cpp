// Copyright Telephone Studios. All Rights Reserved.


#include "Items/BowItem.h"
#include "Components/EquipComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Net/UnrealNetwork.h"

ABowItem::ABowItem()
{
	ProjectileClass = nullptr;
	Drawn = false;
	DefaultEquipPose = nullptr;
	DrawnEquipPose = nullptr;
	EquipPose = nullptr;

	DrawMontage = nullptr;
	DrawItemMontage = nullptr;

	ReleaseMontage = nullptr;
	ReleaseItemMontage = nullptr;
}

void ABowItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABowItem, Drawn, COND_SkipOwner);
}

void ABowItem::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);

	Drawn = false;
	OnRep_Drawn();
}

void ABowItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnPrimaryAnimationClimax(FromFirstPersonInstance);

	PlayFireEffects();
	DecreaseAmmoAndDurability();
}

void ABowItem::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();

	if (!HasAmmo() && GetRemainingAmmoInInventory() <= 0)
	{
		return;
	}

	Reload();

	GetOwnerEquipComponent()->PlayItemMontage(DrawMontage, DrawItemMontage);
	GetOwnerEquipComponent()->StartAim();
}

void ABowItem::OnSecondaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnSecondaryAnimationClimax(FromFirstPersonInstance);
	
	Drawn = true;
	OnRep_Drawn();
}

void ABowItem::OnSecondaryReleased()
{
	Super::OnSecondaryReleased();
	
	GetOwnerEquipComponent()->StopAim();

	if (!Drawn)
	{
		GetOwnerEquipComponent()->StopAllItemMontages();
		return;
	}
	
	// Release Montage
	GetOwnerEquipComponent()->PlayItemMontage(ReleaseMontage, ReleaseItemMontage);

	Drawn = false;
	OnRep_Drawn();
}

bool ABowItem::IsDrawn() const
{
	return Drawn;
}

void ABowItem::OnRep_Drawn()
{
	EquipPose = Drawn ? DrawnEquipPose : DefaultEquipPose;
}
