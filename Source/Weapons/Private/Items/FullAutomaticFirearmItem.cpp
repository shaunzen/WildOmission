// Copyright Telephone Studios. All Rights Reserved.


#include "Items/FullAutomaticFirearmItem.h"
#include "Components/EquipComponent.h"
#include "Kismet/GameplayStatics.h"

void AFullAutomaticFirearmItem::OnPrimaryHeld()
{
	Super::OnPrimaryHeld();

	AActor* OwnerActor = GetOwner();
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerActor == nullptr || OwnerEquipComponent == nullptr || !CanFire)
	{
		return;
	}

	if (!HasAmmo())
	{
		if (OutOfAmmoSound == nullptr)
		{
			return;
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutOfAmmoSound, OwnerActor->GetActorLocation());
		return;
	}
	
	OwnerEquipComponent->PlayItemMontage(FireMontage, FireItemMontage);
}