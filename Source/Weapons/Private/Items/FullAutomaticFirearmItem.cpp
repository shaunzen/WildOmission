// Copyright Telephone Studios. All Rights Reserved.


#include "Items/FullAutomaticFirearmItem.h"
#include "Components/EquipComponent.h"
#include "Kismet/GameplayStatics.h"

void AFullAutomaticFirearmItem::OnPrimaryHeld()
{
	Super::OnPrimaryHeld();

	if (!CanFire)
	{
		return;
	}

	if (!HasAmmo())
	{
		if (OutOfAmmoSound == nullptr)
		{
			return;
		}

		// TODO this could case a crash
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutOfAmmoSound, GetOwner()->GetActorLocation());
		return;
	}
	
	// TODO this could cause a crash
	GetOwnerEquipComponent()->PlayItemMontage(FireMontage, FireItemMontage);
}