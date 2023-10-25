// Copyright Telephone Studios. All Rights Reserved.


#include "Items/MonsterArmsItem.h"
#include "Monsters/Monster.h"
#include "Components/EquipComponent.h"
#include "Engine/DamageEvents.h"

AMonsterArmsItem::AMonsterArmsItem()
{
	DamageMultiplier = 0.3f;
}

void AMonsterArmsItem::OnPrimaryHeld()
{
	Super::OnPrimaryHeld();

	Swing();
}
