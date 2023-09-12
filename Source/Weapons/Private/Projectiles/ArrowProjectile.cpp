// Copyright Telephone Studios. All Rights Reserved.


#include "Projectiles/ArrowProjectile.h"
#include "Components/InventoryComponent.h"

AArrowProjectile::AArrowProjectile()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	DestroyOnImpact = false;
	ItemID = TEXT("arrow.wooden");
}

void AArrowProjectile::Interact(AActor* Interactor)
{
	if (Interactor == nullptr)
	{
		return;
	}

	UInventoryComponent* InteractorInventoryComponent = Interactor->FindComponentByClass<UInventoryComponent>();
	if (InteractorInventoryComponent == nullptr)
	{
		return;
	}

	FInventoryItem ArrowItem;
	ArrowItem.Name = ItemID;
	ArrowItem.Quantity = 1;
	InteractorInventoryComponent->AddItem(ArrowItem);

	this->Destroy();
}

FString AArrowProjectile::PromptText()
{
	return TEXT("Collect Arrow");
}
