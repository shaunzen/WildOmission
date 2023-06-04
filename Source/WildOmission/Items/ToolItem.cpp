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
#include "WildOmission/Core/WildOmissionStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"

AToolItem::AToolItem()
{
	ToolType = EToolType::WOOD;

	GatherMultiplier = 1.0f;
	EffectiveRangeCentimeters = 150.0f;
	SwingSpeedRate = 1.0f;

	Durability = 100;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PrimaryMontageObject(TEXT("/Game/WildOmission/Characters/Human/Animation/Items/A_Human_SwingTool_01_Montage"));
	if (PrimaryMontageObject.Succeeded())
	{
		PrimaryMontage = PrimaryMontageObject.Object;
	}
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

void AToolItem::OnPrimaryHeld()
{
	Super::OnPrimaryHeld();

	if (GetOwner() == nullptr)
	{
		return;
	}

	UEquipComponent* OwnerEquipComponent = GetOwner()->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr || OwnerEquipComponent->PrimaryMontagePlaying())
	{
		return;
	}

	if (GetOwnerCharacter()->IsLocallyControlled())
	{
		OwnerEquipComponent->PlayPrimaryMontage(true);
	}

	Client_PlayThirdPersonPrimaryMontage();
}

void AToolItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetReplicatedControlRotation());

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = true;

	FVector Start = GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		return;
	}
	
	if (FromFirstPersonInstance || !GetOwnerCharacter()->IsLocallyControlled())
	{
		PlayImpactSound(HitResult);
		SpawnImpactParticles(HitResult);
		SpawnImpactDecal(HitResult);
	}

	if (!HasAuthority())
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

	ApplyDamage();
}

UAnimMontage* AToolItem::GetPrimaryMontage() const
{
	return PrimaryMontage;
}

float AToolItem::GetGatherMultiplier() const
{
	return GatherMultiplier;
}

float AToolItem::GetSwingSpeedRate() const
{
	return SwingSpeedRate;
}

void AToolItem::ApplyDamage()
{
	Durability -= 1;

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

void AToolItem::Client_PlayThirdPersonPrimaryMontage_Implementation()
{
	if (GetOwnerCharacter()->IsLocallyControlled())
	{
		return;
	}

	UEquipComponent* OwnerEquipComponent = GetOwner()->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayPrimaryMontage(false);
}

void AToolItem::PlayImpactSound(const FHitResult& HitResult)
{
	USoundBase* ImpactSound = UWildOmissionStatics::GetImpactSoundBySurfaceType(HitResult.PhysMaterial.Get()->SurfaceType);
	if (ImpactSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);
}

void AToolItem::SpawnImpactParticles(const FHitResult& HitResult)
{
	UNiagaraSystem* ImpactEffects = UWildOmissionStatics::GetImpactEffectBySurfaceType(HitResult.PhysMaterial.Get()->SurfaceType);
	if (ImpactEffects == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn impact effects, Impact Effects nullptr."));
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffects, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
}

void AToolItem::SpawnImpactDecal(const FHitResult& HitResult)
{
	UMaterialInterface* DecalMaterial = UWildOmissionStatics::GetImpactDecalBySurfaceType(HitResult.PhysMaterial.Get()->SurfaceType);
	if (DecalMaterial == nullptr)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawning Decal"));
	UGameplayStatics::SpawnDecalAttached(DecalMaterial, FVector(2.0f, 10.0f, 10.0f), HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
}