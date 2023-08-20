// Copyright Telephone Studios. All Rights Reserved.


#include "Components/HarvestableComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Log.h"

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

void UHarvestableComponent::OnHarvest(AActor* HarvestingActor, float GatherMultiplier)
{
	if (HarvestingActor == nullptr)
	{
		UE_LOG(LogGatherableResources, Warning, TEXT("Could not harvest resource, HarvestingActor was nullptr."));
		return;
	}

	UInventoryComponent* HarvestingInventoryComponent = HarvestingActor->FindComponentByClass<UInventoryComponent>();
	if (HarvestingInventoryComponent == nullptr)
	{
		UE_LOG(LogGatherableResources, Warning, TEXT("Could not harvest resource, HarvestingActor has no InventoryComponent."));
		return;
	}

	HarvestingInventoryComponent->AddItem(HandleYield(GatherMultiplier), nullptr, true);
	Durability--;

	if (Durability <= 0)
	{
		GetOwner()->Destroy();
	}
}

FInventoryItem UHarvestableComponent::HandleYield(float GatherMultiplier)
{
	FInventoryItem ItemToGive;
	ItemToGive = ItemYield;
	ItemToGive.Quantity = ItemYield.Quantity * GatherMultiplier;

	if (ItemToGive.Quantity <= 0)
	{
		ItemToGive.Quantity = 1;
	}

	return ItemToGive;
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