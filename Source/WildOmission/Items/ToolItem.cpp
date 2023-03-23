// Copyright Telephone Studios. All Rights Reserved.


#include "ToolItem.h"
#include "UObject/ConstructorHelpers.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/EquipComponent.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Components/HarvestableComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AToolItem::AToolItem()
{
	ToolType = EToolType::WOOD;

	GatherMultiplyer = 1.0f;
	EffectiveRangeCentimeters = 150.0f;
	SwingTimeSeconds = 1.0f;

	Durability = 1000;

	ConstructorHelpers::FObjectFinder<USoundBase> HarvestSoundObject(TEXT("/Game/WildOmission/Items/EquipableItems/Audio/Tools/WoodImpact_Cue"));
	if (HarvestSoundObject.Object == nullptr)
	{
		return;
	}

	HarvestSound = HarvestSoundObject.Object;
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
}

void AToolItem::OnUnequip()
{
	Super::OnUnequip();

}

void AToolItem::Primary()
{
	Super::Primary();

	Client_PlaySwingAnimation();
}

void AToolItem::Harvest()
{
	if (!HasAuthority())
	{
		return;
	}
	
	FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetControlRotation());

	FHitResult HitResult;
	FVector Start = GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		UHarvestableComponent* HitHarvestableComponent = HitResult.GetActor()->FindComponentByClass<UHarvestableComponent>();

		if (HitHarvestableComponent && (HitHarvestableComponent->GetRequiredToolType() == ToolType || ToolType == EToolType::MULTI))
		{
			HitHarvestableComponent->OnHarvest(GetOwner());
		}

		Client_PlayHarvestSound();
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

void AToolItem::Client_PlayHarvestSound_Implementation()
{
	if (HarvestSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HarvestSound, GetActorLocation());
}