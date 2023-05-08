// Copyright Telephone Studios. All Rights Reserved.


#include "HarvestableResource.h"
#include "WildOmission/Components/EquipComponent.h"
#include "WildOmission/Items/ToolItem.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"

// Sets default values
AHarvestableResource::AHarvestableResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	RootComponent = MeshComponent;

	TotalYield = 100;
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

	TotalYield -= ItemYield.Quantity;

	if (TotalYield <= 0.0f)
	{
		Destroy();
	}
}

TEnumAsByte<EToolType> AHarvestableResource::GetRequiredToolType() const
{
	return RequiredToolType;
}

int32 AHarvestableResource::GetTotalYield() const
{
	return TotalYield;
}

void AHarvestableResource::SetTotalYield(const int32& InTotalYield)
{
	TotalYield = InTotalYield;
}