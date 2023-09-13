// Copyright Telephone Studios. All Rights Reserved.


#include "Projectiles/ArrowProjectile.h"
#include "Components/InventoryComponent.h"

AArrowProjectile::AArrowProjectile()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}