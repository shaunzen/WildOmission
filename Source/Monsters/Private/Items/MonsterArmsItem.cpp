// Copyright Telephone Studios. All Rights Reserved.


#include "Items/MonsterArmsItem.h"
#include "Monsters/Monster.h"
#include "Components/EquipComponent.h"
#include "Engine/DamageEvents.h"

AMonsterArmsItem::AMonsterArmsItem()
{
}

void AMonsterArmsItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	UE_LOG(LogTemp, Warning, TEXT("Primary"));
	if (!HasAuthority())
	{
		return;
	}

	AMonster* OwnerMonster = Cast<AMonster>(GetOwner());
	if (OwnerMonster == nullptr)
	{
		return;
	}

	APawn* TargetPawn = OwnerMonster->GetTargetPawn();
	if (TargetPawn == nullptr || FVector::Distance(this->GetActorLocation(), TargetPawn->GetActorLocation()) > EffectiveRangeCentimeters)
	{
		return;
	}

	FPointDamageEvent HitByMonsterEvent(10.0f, FHitResult(), OwnerMonster->GetActorForwardVector(), nullptr);
	TargetPawn->TakeDamage(10.0f, HitByMonsterEvent, OwnerMonster->GetController(), this);
}
