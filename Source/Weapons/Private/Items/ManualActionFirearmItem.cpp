// Copyright Telephone Studios. All Rights Reserved.


#include "Items/ManualActionFirearmItem.h"
#include "Components/EquipComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AManualActionFirearmItem::AManualActionFirearmItem()
{
    Chambered = false;
    ChamberMontage = nullptr;
    ChamberItemMontage = nullptr;
}

void AManualActionFirearmItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AManualActionFirearmItem, Chambered, COND_OwnerOnly);
}

void AManualActionFirearmItem::OnPrimaryPressed()
{
    Super::OnPrimaryPressed();

    AActor* OwnerActor = GetOwner();
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerActor == nullptr || OwnerEquipComponent == nullptr || !CanFire)
	{
		return;
	}

	if (!HasAmmo() || Chambered == false)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutOfAmmoSound, OwnerActor->GetActorLocation());
		return;
	}

	OwnerEquipComponent->PlayItemMontage(FireMontage, FireItemMontage);
}

void AManualActionFirearmItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
    Super::OnPrimaryAnimationClimax(FromFirstPersonInstance);

    Chambered = false;

    AActor* OwnerActor = GetOwner();
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerActor == nullptr || OwnerEquipComponent == nullptr)
	{
		return;
	}

	if (!HasAmmo())
	{
		return;
	}

	OwnerEquipComponent->PlayItemMontage(ChamberMontage, ChamberItemMontage);
}

void AManualActionFirearmItem::OnChamberedAnimationClimax(bool FromFirstPersonInstance)
{
    Super::OnChamberedAnimationClimax(FromFirstPersonInstance);

    if (!HasAuthority())
    {
        return;
    }

    Chambered = true;
}