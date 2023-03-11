// Copyright Telephone Studios. All Rights Reserved.


#include "ToolItem.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/EquipComponent.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Components/HarvestableComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

AToolItem::AToolItem()
{
	ToolType = EToolType::WOOD;

	GatherMultiplyer = 1.0f;
	EffectiveRangeCentimeters = 100.0f;
	SwingTimeSeconds = 1.0f;

	Durability = 1000;
}

void AToolItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AToolItem, Durability);
}

void AToolItem::Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerCharacter, InItemName, InFromSlotIndex, InUniqueID);

	UPlayerInventoryComponent* OwnerInventory = Owner->FindComponentByClass<UPlayerInventoryComponent>();
	if (OwnerInventory == nullptr)
	{
		return;
	}

	FInventoryItem* InventoryItem = OwnerInventory->FindItemWithUniqueID(UniqueID);
	if (InventoryItem == nullptr)
	{
		return;
	}

	Durability = InventoryItem->GetStat(FName("Durability"));

	UE_LOG(LogTemp, Warning, TEXT("Pickup Durability %i"), Durability);
}

void AToolItem::OnUnequip()
{
	Super::OnUnequip();

}

void AToolItem::Primary()
{
	Super::Primary();

	Client_PlaySwingAnimation();
	
	FHitResult HitResult;
	FVector Start = GetOwnerCharacter()->GetCameraOrigin();
	FVector End = Start + (GetOwnerCharacter()->GetCameraForwardVector() * EffectiveRangeCentimeters);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		UHarvestableComponent* HitHarvestableComponent = HitResult.GetActor()->FindComponentByClass<UHarvestableComponent>();
		
		if (HitHarvestableComponent == nullptr || HitHarvestableComponent->GetRequiredToolType() != ToolType)
		{
			return;
		}

		HitHarvestableComponent->OnHarvest(GetOwner());
		ApplyDamage();
	}
}

void AToolItem::Secondary()
{
	Super::Secondary();
}

void AToolItem::ApplyDamage()
{
	Durability -= 10;

	FInventoryItem* InventoryItem = FindInInventory();
	InventoryItem->SetStat(FName("Durability"), Durability);

	if (Durability <= 0.0f)
	{
		//remove held item
		UPlayerInventoryComponent* OwnerInventory = Owner->FindComponentByClass<UPlayerInventoryComponent>();
		if (OwnerInventory == nullptr)
		{
			return;
		}

		OwnerInventory->RemoveHeldItem();
	}
}

FInventoryItem* AToolItem::FindInInventory()
{
	UInventoryComponent* OwnerInventory = Owner->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventory == nullptr)
	{
		return nullptr;
	}

	FInventoryItem* InventoryItem = OwnerInventory->FindItemWithUniqueID(UniqueID);
	if (InventoryItem == nullptr)
	{
		if (!OwnerInventory->GetManipulator()->SelectedItemHasUniqueID(UniqueID))
		{
			return nullptr;
		}

		InventoryItem = OwnerInventory->GetManipulator()->GetSelectedItemAddress();
	}

	return InventoryItem;
}

void AToolItem::Client_PlaySwingAnimation_Implementation()
{
	UEquipComponent* OwnerEquipComponent = GetOwner()->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlaySwingAnimation();
}