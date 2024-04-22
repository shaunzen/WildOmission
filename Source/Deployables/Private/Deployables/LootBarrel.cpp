// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/LootBarrel.h"
#include "Components/InventoryComponent.h"

ALootBarrel::ALootBarrel()
{

}

void ALootBarrel::OnDeployableDestroyed()
{
    Super::OnDeployableDestroyed();

    DropLoot();
}

void ALootBarrel::DropLoot()
{
    const int32 SpawnCount = FMath::RandRange(1,3);
    for (int32 i = 0; i < SpawnCount; ++i)
    {
        const int32 LootIndex = FMath::RandRange(0, Drops.Num() - 1);
        if (!Drops.IsValidIndex(LootIndex))
        {
            return;
        }
        
        UInventoryComponent::SpawnWorldItem(GetWorld(), Drops[LootIndex], this);
    }
}