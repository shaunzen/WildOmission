// Copyright Telephone Studios. All Rights Reserved.


#include "Items/SemiAutomaticFirearmItem.h"
#include "Components/EquipComponent.h"
#include "Kismet/GameplayStatics.h"

void ASemiAutomaticFirearmItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

	AActor* OwnerActor = GetOwner();
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerActor == nullptr || OwnerEquipComponent == nullptr || !CanFire)
	{
		return;
	}

	if (!HasAmmo())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutOfAmmoSound, OwnerActor->GetActorLocation());
		return;
	}

	OwnerEquipComponent->PlayItemMontage(FireMontage, FireItemMontage);
}