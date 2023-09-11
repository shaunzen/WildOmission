// Copyright Telephone Studios. All Rights Reserved.


#include "Items/SemiAutomaticFirearmItem.h"
#include "Components/EquipComponent.h"
#include "Kismet/GameplayStatics.h"

void ASemiAutomaticFirearmItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

	if (!HasAmmo())
	{
		if (OutOfAmmoSound == nullptr)
		{
			return;
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutOfAmmoSound, GetOwner()->GetActorLocation());
		return;
	}

	GetOwnerEquipComponent()->PlayItemMontage(PrimaryMontage, PrimaryItemMontage);
}
