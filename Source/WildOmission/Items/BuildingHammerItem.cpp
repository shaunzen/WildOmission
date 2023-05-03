// Copyright Telephone Studios. All Rights Reserved.


#include "BuildingHammerItem.h"
#include "UObject/ConstructorHelpers.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/EquipComponent.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Deployables/Deployable.h"
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

void ABuildingHammerItem::OnPrimaryAnimationClimax()
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

	AWildOmissionCharacter* HitCharacter = Cast<AWildOmissionCharacter>(HitResult.GetActor());
	if (HitCharacter)
	{
		FPointDamageEvent HitByToolEvent(20.0f, HitResult, OwnerCharacterLookVector, nullptr);
		HitCharacter->TakeDamage(20.0f, HitByToolEvent, GetOwnerCharacter()->GetController(), this);
	}

	ADeployable* HitDeployable = Cast<ADeployable>(HitResult.GetActor());
	if (HitDeployable)
	{
		float DamageAmount = 50.0f;

		FPointDamageEvent HitByToolEvent(DamageAmount, HitResult, OwnerCharacterLookVector, nullptr);
		HitDeployable->TakeDamage(DamageAmount, HitByToolEvent, GetOwnerCharacter()->GetController(), this);
	}

	Client_PlayHitSound(HitResult.ImpactPoint);
	ApplyDamage();
}

bool ABuildingHammerItem::GetLookingAtItemDurability(float& OutPercent) const
{
	FHitResult HitResult;
	
	FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetControlRotation());
	FVector Start = GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
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