// Copyright Telephone Studios. All Rights Reserved.


#include "ToolItem.h"
#include "UObject/ConstructorHelpers.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/EquipComponent.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Resources/HarvestableResource.h"
#include "WildOmission/Deployables/Deployable.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"

AToolItem::AToolItem()
{
	ToolType = EToolType::WOOD;

	GatherMultiplier = 1.0f;
	EffectiveRangeCentimeters = 150.0f;
	SwingTimeSeconds = 1.0f;

	Durability = 1000;

	ConstructorHelpers::FObjectFinder<USoundBase> HitSoundObject(TEXT("/Game/WildOmission/Items/EquipableItems/Audio/Tools/WoodImpact_Cue"));
	ConstructorHelpers::FObjectFinder<UAnimMontage> PrimaryMontageObject(TEXT("/Game/WildOmission/Characters/Human/Animation/Items/A_Human_SwingTool_Montage"));

	if (HitSoundObject.Object == nullptr || PrimaryMontageObject.Object == nullptr)
	{
		return;
	}

	HitSound = HitSoundObject.Object;
	PrimaryMontage = PrimaryMontageObject.Object;
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

	Client_PlayPrimaryMontage();
}

void AToolItem::OnPrimaryAnimationClimax()
{
	if (!HasAuthority())
	{
		return;
	}
	
	FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetControlRotation());

	FHitResult HitResult;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	FVector Start = GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		return;
	}

	AHarvestableResource* HitHarvestable = Cast<AHarvestableResource>(HitResult.GetActor());

	if (HitHarvestable && (HitHarvestable->GetRequiredToolType() == ToolType || ToolType == EToolType::MULTI))
	{
		HitHarvestable->OnHarvest(GetOwner());
	}

	AWildOmissionCharacter* HitCharacter = Cast<AWildOmissionCharacter>(HitResult.GetActor());
	if (HitCharacter)
	{
		FPointDamageEvent HitByToolEvent(20.0f, HitResult, OwnerCharacterLookVector, nullptr);
		HitCharacter->TakeDamage(20.0f, HitByToolEvent, GetOwnerCharacter()->GetController(), this);
	}

	ADeployable* HitDeployable = Cast<ADeployable>(HitResult.GetActor());
	if (HitDeployable)
	{
		float DamageAmount = 5.0f;

		FPointDamageEvent HitByToolEvent(DamageAmount * GatherMultiplier, HitResult, OwnerCharacterLookVector, nullptr);
		HitDeployable->TakeDamage(DamageAmount * GatherMultiplier, HitByToolEvent, GetOwnerCharacter()->GetController(), this);
	}

	Client_PlayHitSound(HitResult.ImpactPoint);
	ApplyDamage();
}

void AToolItem::Secondary()
{
	Super::Secondary();
}

UAnimMontage* AToolItem::GetPrimaryMontage() const
{
	return PrimaryMontage;
}

float AToolItem::GetGatherMultiplier() const
{
	return GatherMultiplier;
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

// TODO make static
FInventoryItem* AToolItem::FindInInventory()
{
	UInventoryComponent* OwnerInventory = Owner->FindComponentByClass<UInventoryComponent>();
	UInventoryManipulatorComponent* OwnerInventoryManipulatorComponent = Owner->FindComponentByClass<UInventoryManipulatorComponent>();
	if (OwnerInventory == nullptr || OwnerInventoryManipulatorComponent == nullptr)
	{
		return nullptr;
	}

	FInventoryItem* InventoryItem = OwnerInventory->FindItemWithUniqueID(UniqueID);
	if (InventoryItem == nullptr)
	{
		if (!OwnerInventoryManipulatorComponent->SelectedItemHasUniqueID(UniqueID))
		{
			return nullptr;
		}

		InventoryItem = OwnerInventoryManipulatorComponent->GetSelectedItemAddress();
	}

	return InventoryItem;
}

void AToolItem::Client_PlayPrimaryMontage_Implementation()
{
	UEquipComponent* OwnerEquipComponent = GetOwner()->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayPrimaryMontage();
}

void AToolItem::Client_PlayHitSound_Implementation(const FVector& HitLocation)
{
	if (HitSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitLocation);
}