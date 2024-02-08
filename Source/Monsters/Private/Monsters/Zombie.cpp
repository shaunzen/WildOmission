// Copyright Telephone Studios. All Rights Reserved.


#include "Monsters/Zombie.h"
#include "Components/PlayerInventoryComponent.h"

AZombie::AZombie()
{
	DefaultWalkSpeed = 550.0f;
}

void AZombie::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}

	FInventoryItem ZombieArmsItem;
	ZombieArmsItem.Name = TEXT("zombie.arms");
	ZombieArmsItem.Quantity = 1;
	InventoryComponent->AddItem(ZombieArmsItem);
	InventoryComponent->SetToolbarSelectionIndex(2);
}
