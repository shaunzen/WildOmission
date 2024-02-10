// Copyright Telephone Studios. All Rights Reserved.


#include "Monsters/Zombie.h"
#include "Components/PlayerInventoryComponent.h"

AZombie::AZombie()
{
	DefaultWalkSpeed = 550.0f;
	ItemToEquip.Name = TEXT("zombie.arms");
	ItemToEquip.Quantity = 1;
}