// Copyright Telephone Studios. All Rights Reserved.


#include "HarvestableResource.h"
#include "WildOmission/Components/EquipComponent.h"
#include "WildOmission/Items/ToolItem.h"
#include "WildOmission/Components/InventoryComponent.h"

// Sets default values
AHarvestableResource::AHarvestableResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	RootComponent = MeshComponent;

	Identifier = FName();
}

void AHarvestableResource::OnHarvest(AActor* HarvestingActor)
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
		Destroy();
	}
}

TEnumAsByte<EToolType> AHarvestableResource::GetRequiredToolType() const
{
	return RequiredToolType;
}

int32 AHarvestableResource::GetDurability() const
{
	return Durability;
}

void AHarvestableResource::SetDurability(const int32& InDurability)
{
	Durability = InDurability;
}

FName AHarvestableResource::GetIdentifier() const
{
	return Identifier;
}

void AHarvestableResource::SetIdentifier(const FName& InIdentifier)
{
	Identifier = InIdentifier;
}