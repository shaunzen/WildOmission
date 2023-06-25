// Copyright Telephone Studios. All Rights Reserved.


#include "Items/BuildingHammerItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/EquipComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Interfaces/DurabilityInterface.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"

ABuildingHammerItem::ABuildingHammerItem()
{
	ToolType = BUILD;
	EffectiveRangeCentimeters = 300.0f;
}

void ABuildingHammerItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(GetOwnerPawn()->GetControlRotation());

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

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
	}

	if (!HasAuthority())
	{
		return;
	}

	// TODO add this back later

	ApplyDamage();
}

bool ABuildingHammerItem::GetLookingAtItemDurability(float& OutPercent) const
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

	OutPercent = DurabilityInterfaceActor->GetDurabilityPercentage();
	return true;
}