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
	if (!HasAuthority() || GetOwnerPawn() == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	const FVector Start = GetOwner()->GetActorLocation();
	const FVector ForwardVector = GetOwner()->GetActorForwardVector();
	const FVector End = Start + (ForwardVector * EffectiveRangeCentimeters);
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
	{
		return;
	}
	
	APawn* HitPawn = Cast<APawn>(HitResult.GetActor());
	if (HitPawn)
	{
		FPointDamageEvent HitByMonsterEvent(30.0f, HitResult, GetOwner()->GetActorForwardVector(), nullptr);
		HitPawn->TakeDamage(30.0f, HitByMonsterEvent, GetOwnerPawn()->GetController(), this);
	}
}
