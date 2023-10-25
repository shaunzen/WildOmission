// Copyright Telephone Studios. All Rights Reserved.


#include "Items/SwingableItem.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "SurfaceHelpers.h"
#include "Engine/DamageEvents.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WildOmissionGameUserSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

ASwingableItem::ASwingableItem()
{
	SwingMontage = nullptr;
	SwingItemMontage = nullptr;
	DamageMultiplier = 1.0f;
	EffectiveRangeCentimeters = 150.0f;
	SwingSpeedRate = 1.0f;
	SwingCameraShake = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SwingMontageObject(TEXT("/Game/WildOmissionCore/Characters/Human/Animation/Items/A_Human_Tool_Swing_01_Montage"));
	if (SwingMontageObject.Succeeded())
	{
		SwingMontage = SwingMontageObject.Object;
	}

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> DefaultSwingCameraShakeBlueprint(TEXT("/Game/EquipableItems/Effects/CS_ToolSwing"));
	if (DefaultSwingCameraShakeBlueprint.Succeeded())
	{
		SwingCameraShake = DefaultSwingCameraShakeBlueprint.Class;
	}
}

void ASwingableItem::Swing()
{
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayItemMontage(SwingMontage, SwingItemMontage);
}

void ASwingableItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnPrimaryAnimationClimax(FromFirstPersonInstance);

	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	PlayCameraShake();

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
		UE_LOG(LogEquipableItems, Verbose, TEXT("Nothing was hit by tool."));
		return;
	}

	OnSwingImpact(HitResult, OwnerCharacterLookVector, FromFirstPersonInstance);
}

void ASwingableItem::OnSwingImpact(const FHitResult& HitResult, const FVector& OwnerCharacterLookVector, bool FromFirstPersonInstance)
{
	if (FromFirstPersonInstance || !IsOwnerOurPlayer())
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
		FPointDamageEvent HitByToolEvent(20.0f * DamageMultiplier, HitResult, OwnerCharacterLookVector, nullptr);
		HitPawn->TakeDamage(20.0f * DamageMultiplier, HitByToolEvent, GetOwnerPawn()->GetController(), this);
	}
	else
	{
		float DamageAmount = 5.0f;

		FPointDamageEvent HitByToolEvent(DamageAmount * DamageMultiplier, HitResult, OwnerCharacterLookVector, nullptr);
		HitResult.GetActor()->TakeDamage(DamageAmount * DamageMultiplier, HitByToolEvent, GetOwnerPawn()->GetController(), this);
	}

	DecrementDurability();
}

void ASwingableItem::DecrementDurability()
{
	AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	UPlayerInventoryComponent* OwnerInventory = Owner->FindComponentByClass<UPlayerInventoryComponent>();
	if (OwnerInventory == nullptr)
	{
		return;
	}

	FInventorySlot* FromSlot = OwnerInventory->GetSlot(GetFromSlotIndex());
	if (FromSlot == nullptr)
	{
		return;
	}

	const int32 CurrentDurability = FromSlot->Item.GetStat(TEXT("Durability"));
	const int32 NewDurability = CurrentDurability - 1;

	FromSlot->Item.SetStat(TEXT("Durability"), NewDurability);

	OwnerInventory->RequestInventoryRefresh();
}

void ASwingableItem::PlayImpactSound(const FHitResult& HitResult)
{
	if (HitResult.PhysMaterial == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Failed to get physical material."));
		return;
	}

	USoundBase* ImpactSound = USurfaceHelpers::GetImpactSound(HitResult.PhysMaterial.Get()->SurfaceType);
	if (ImpactSound == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Failed to get impact sound."));
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);
}

void ASwingableItem::SpawnImpactParticles(const FHitResult& HitResult, const FVector& ImpactorForwardVector)
{
	if (HitResult.PhysMaterial == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Failed to get physical material."));
		return;
	}

	UNiagaraSystem* ImpactParticles = USurfaceHelpers::GetImpactParticles(HitResult.PhysMaterial->SurfaceType);
	if (ImpactParticles == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Failed to spawn impact effects, Impact Effects nullptr."));
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticles, HitResult.ImpactPoint, (-ImpactorForwardVector).Rotation());
}

void ASwingableItem::SpawnImpactDecal(const FHitResult& HitResult)
{
	if (HitResult.PhysMaterial == nullptr)
	{
		UE_LOG(LogEquipableItems, Warning, TEXT("Cannot spawn impact decal, Failed to get physical material."));
		return;
	}

	UMaterialInterface* DecalMaterial = USurfaceHelpers::GetImpactDecal(HitResult.PhysMaterial->SurfaceType);
	if (DecalMaterial == nullptr)
	{
		return;
	}

	FVector DecalSize = FVector(8.0f, 15.0f, 15.0f);
	UGameplayStatics::SpawnDecalAttached(DecalMaterial, DecalSize, HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 120.0f);

}

void ASwingableItem::PlayCameraShake()
{
	if (SwingCameraShake == nullptr || GetOwnerPawn() == nullptr || !GetOwnerPawn()->IsLocallyControlled())
	{
		return;
	}

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr || UserSettings->GetCameraShakeEnabled() == false)
	{
		return;
	}

	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwnerPawn()->GetController());
	if (OwnerPlayerController == nullptr)
	{
		return;
	}

	OwnerPlayerController->ClientStartCameraShake(SwingCameraShake);
}
