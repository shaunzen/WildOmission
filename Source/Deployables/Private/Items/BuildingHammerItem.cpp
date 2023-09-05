// Copyright Telephone Studios. All Rights Reserved.


#include "Items/BuildingHammerItem.h"
#include "Components/EquipComponent.h"
#include "Interfaces/DurabilityInterface.h"
#include "Components/InventoryComponent.h"
#include "Deployables/Deployable.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"

ABuildingHammerItem::ABuildingHammerItem()
{
	ToolType = BUILD;
	EffectiveRangeCentimeters = 300.0f;
	MaxRepairAmount = 15.0f;
}

void ABuildingHammerItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	const UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	const FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(OwnerEquipComponent->GetOwnerControlRotation());

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = true;

	const FVector Start = GetOwnerPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	const FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		return;
	}

	if (FromFirstPersonInstance || !GetOwnerPawn()->IsLocallyControlled())
	{
		PlayImpactSound(HitResult);
		SpawnImpactParticles(HitResult, OwnerCharacterLookVector);
		//SpawnImpactDecal(HitResult);
	}

	if (!HasAuthority())
	{
		return;
	}
	
	ADeployable* HitDeployable = Cast<ADeployable>(HitResult.GetActor());
	if (HitDeployable)
	{
		AttemptDeployableRepair(HitDeployable, HitResult, OwnerCharacterLookVector);
		UpdateDurability();
	}

}

bool ABuildingHammerItem::GetLookingAtItemDurability(float& OutCurrentDurability, float& OutMaxDurability, FString& OutActorName) const
{
	if (GetOwnerPawn() == nullptr)
	{
		return false;
	}
	
	FHitResult HitResult;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(GetOwnerPawn()->GetControlRotation());
	FVector Start = GetOwnerPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		return false;
	}

	IDurabilityInterface* DurabilityInterfaceActor = Cast<IDurabilityInterface>(HitResult.GetActor());
	if (DurabilityInterfaceActor == nullptr)
	{
		return false;
	}
	
	OutCurrentDurability = DurabilityInterfaceActor->GetCurrentDurability();
	OutMaxDurability = DurabilityInterfaceActor->GetMaxDurability();
	OutActorName = HitResult.GetActor()->GetActorNameOrLabel();
	return true;
}

void ABuildingHammerItem::AttemptDeployableRepair(ADeployable* DeployableToRepair, const FHitResult& HitResult, const FVector& DirectionVector)
{
	// Check if this deployable is damaged
	if (DeployableToRepair->GetCurrentDurability() >= DeployableToRepair->GetMaxDurability())
	{
		return;
	}

	// Get material type
	FInventoryItem ItemRepairCost = GetBaseRepairCostForDeployable(DeployableToRepair);

	// Check if the player has this amount of resources?
	UInventoryComponent* OwnerInventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventoryComponent == nullptr || !OwnerInventoryComponent->GetContents()->HasItem(ItemRepairCost.Name))
	{
		return;
	}

	// if the deployable needs less than we are willing to give, we need to cut back and only use the amount required
	const float DurabilityDifference = DeployableToRepair->GetMaxDurability() - DeployableToRepair->GetCurrentDurability();
	if (DurabilityDifference < ItemRepairCost.Quantity)
	{
		ItemRepairCost.Quantity = DurabilityDifference;
	}

	// if the player is lacking sufficient resources we must reduce the amount of repair performed
	const int32 PlayerResourceQuantity = OwnerInventoryComponent->GetContents()->GetItemQuantity(ItemRepairCost.Name);
	if (PlayerResourceQuantity < ItemRepairCost.Quantity)
	{
		ItemRepairCost.Quantity = PlayerResourceQuantity;
	}


	const float RepairAmount = -(ItemRepairCost.Quantity);

	FPointDamageEvent HitByToolEvent(RepairAmount, HitResult, DirectionVector, nullptr);
	DeployableToRepair->TakeDamage(RepairAmount, HitByToolEvent, GetOwnerPawn()->GetController(), this);

}

FInventoryItem ABuildingHammerItem::GetBaseRepairCostForDeployable(ADeployable* DeployableToRepair) const
{
	FInventoryItem ItemToReturn;
	if (DeployableToRepair == nullptr)
	{
		return ItemToReturn;
	}

	switch (DeployableToRepair->GetMaterialType())
	{
	case EToolType::WOOD:
		ItemToReturn.Name = TEXT("wood");
		break;
	case EToolType::STONE:
		ItemToReturn.Name = TEXT("stone");
		break;
	case EToolType::METAL:
		ItemToReturn.Name = TEXT("metal");
		break;
	default:
		ItemToReturn.Name = TEXT("wood");
		break;
	}

	ItemToReturn.Quantity = MaxRepairAmount;

	return ItemToReturn;
}
