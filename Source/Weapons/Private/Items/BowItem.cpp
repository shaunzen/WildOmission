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

void ABowItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnPrimaryAnimationClimax(FromFirstPersonInstance);

	PlayFireEffects();
	DecreaseAmmoAndDurability();
}

void ABowItem::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();

	if (GetRemainingAmmoInInventory() <= 0)
	{
		return;
	}

	Reload();
	GetOwnerEquipComponent()->PlayItemMontage(SecondaryMontage, SecondaryItemMontage);
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
	
	GetOwnerEquipComponent()->PlayItemMontage(PrimaryMontage, PrimaryItemMontage);

	EquipPose = DefaultEquipPose;
	AtFullDraw = false;
}