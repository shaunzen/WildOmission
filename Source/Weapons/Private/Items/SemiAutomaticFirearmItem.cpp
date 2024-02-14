// Copyright Telephone Studios. All Rights Reserved.


#include "Items/SemiAutomaticFirearmItem.h"
#include "Components/EquipComponent.h"
#include "Kismet/GameplayStatics.h"

void ASemiAutomaticFirearmItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

	UWorld* World = GetWorld();
	AActor* OwnerActor = GetOwner();
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (World == nullptr || OwnerActor == nullptr || OwnerEquipComponent == nullptr || !CanFire)
	{
		return;
	}

	if (!HasAmmo())
	{
		if (OutOfAmmoSound == nullptr)
		{
			return;
		}
		
		UGameplayStatics::PlaySoundAtLocation(World, OutOfAmmoSound, OwnerActor->GetActorLocation());
		return;
	}

	OwnerEquipComponent->PlayItemMontage(FireMontage, FireItemMontage);
}