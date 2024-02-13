// Copyright Telephone Studios. All Rights Reserved.


#include "Items/SemiAutomaticFirearmItem.h"
#include "Components/EquipComponent.h"
#include "Kismet/GameplayStatics.h"

void ASemiAutomaticFirearmItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

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
		
		// TODO this could cause a crash
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutOfAmmoSound, GetOwner()->GetActorLocation());
		return;
	}

	// TODO this could cause a crash
	GetOwnerEquipComponent()->PlayItemMontage(FireMontage, FireItemMontage);
}