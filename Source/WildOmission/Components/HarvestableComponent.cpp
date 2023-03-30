// Copyright Telephone Studios. All Rights Reserved.


#include "HarvestableComponent.h"
#include "InventoryComponent.h"

UHarvestableComponent::UHarvestableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

}

void UHarvestableComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHarvestableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHarvestableComponent::OnHarvest(AActor* HarvestingActor)
{
	UInventoryComponent* HarvestingInventoryComponent = HarvestingActor->FindComponentByClass<UInventoryComponent>();
	if (HarvestingInventoryComponent == nullptr)
	{
		return;
	}

	FInventoryItem ItemToGive;
	ItemToGive = ItemYeild;
	ItemToGive.Quantity = ItemYeild.Quantity * 1.0f; // todo make tool gather multiplier

	if (ItemToGive.Quantity <= 0)
	{
		ItemToGive.Quantity = 1;
	}

	HarvestingInventoryComponent->AddItem(ItemToGive);

	Durability -= ItemToGive.Quantity;

	if (Durability <= 0.0f)
	{
		GetOwner()->Destroy();
	}
}

TEnumAsByte<EToolType> UHarvestableComponent::GetRequiredToolType() const
{
	return RequiredToolType;
}

int32 UHarvestableComponent::GetDurability() const
{
	return Durability;
}

void UHarvestableComponent::SetDurability(const int32& InDurability)
{
	Durability = InDurability;
}

FName UHarvestableComponent::GetType() const
{
	return Type;
}