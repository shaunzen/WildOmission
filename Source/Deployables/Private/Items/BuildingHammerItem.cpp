// Copyright Telephone Studios. All Rights Reserved.


#include "Items/BuildingHammerItem.h"
#include "Components/EquipComponent.h"
#include "Interfaces/DurabilityInterface.h"
#include "Deployables/Deployable.h"
#include "Deployables/BuildingPart.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"

ABuildingHammerItem::ABuildingHammerItem()
{
	ToolType = BUILD;
	EffectiveRangeCentimeters = 300.0f;
}

void ABuildingHammerItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(OwnerEquipComponent->GetOwnerControlRotation());

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = true;

	FVector Start = GetOwnerPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		return;
	}

	if (FromFirstPersonInstance || !GetOwnerPawn()->IsLocallyControlled())
	{
		PlayImpactSound(HitResult);
		SpawnImpactParticles(HitResult, OwnerCharacterLookVector);
		SpawnImpactDecal(HitResult);
	}

	if (!HasAuthority())
	{
		return;
	}

	APawn* HitPawn = Cast<APawn>(HitResult.GetActor());
	if (HitPawn)
	{
		FPointDamageEvent HitByToolEvent(20.0f, HitResult, OwnerCharacterLookVector, nullptr);
		HitPawn->TakeDamage(20.0f, HitByToolEvent, GetOwnerPawn()->GetController(), this);
	}

	ADeployable* HitDeployable = Cast<ADeployable>(HitResult.GetActor());
	if (HitDeployable)
	{
		float DamageAmount = 50.0f;

		FPointDamageEvent HitByToolEvent(DamageAmount, HitResult, OwnerCharacterLookVector, nullptr);
		HitDeployable->TakeDamage(DamageAmount, HitByToolEvent, GetOwnerPawn()->GetController(), this);
	}

	ApplyDamage();
}

bool ABuildingHammerItem::GetLookingAtItemDurability(float& OutCurrentDurability, float& OutMaxDurability, FString& OutActorName) const
{
	FHitResult HitResult;
	if (!LineTraceOnVisibility(HitResult))
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

bool ABuildingHammerItem::GetLookingAtItemStability(float& OutStability) const
{
	FHitResult HitResult;
	if (!LineTraceOnVisibility(HitResult))
	{
		return false;
	}

	ABuildingPart* HitBuildingPart = Cast<ABuildingPart>(HitResult.GetActor());
	if (HitBuildingPart == nullptr)
	{
		return false;
	}

	OutStability = HitBuildingPart->GetStability();

	return true;
}

bool ABuildingHammerItem::LineTraceOnVisibility(FHitResult& OutHitResult) const
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

	return GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);
}
