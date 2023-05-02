// Copyright Telephone Studios. All Rights Reserved.


#include "HarvestableComponent.h"
#include "EquipComponent.h"
#include "WildOmission/Items/ToolItem.h"
#include "InventoryComponent.h"
#include "InventoryManipulatorComponent.h"

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
	UEquipComponent* HarvestingEquipComponent = HarvestingActor->FindComponentByClass<UEquipComponent>();
	if (HarvestingInventoryComponent == nullptr || HarvestingEquipComponent == nullptr)
	{
		return;
	}

	AToolItem* HarvestingTool = Cast<AToolItem>(HarvestingEquipComponent->GetEquipedItem());
	if (HarvestingTool == nullptr)
	{
		return;
	}

	FInventoryItem ItemToGive;
	ItemToGive = ItemYield;
	ItemToGive.Quantity = ItemYield.Quantity * HarvestingTool->GetGatherMultiplier();

	if (ItemToGive.Quantity <= 0)
	{
		ItemToGive.Quantity = 1;
	}

	HarvestingInventoryComponent->AddItem(ItemToGive);

	Durability -= ItemYield.Quantity;

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