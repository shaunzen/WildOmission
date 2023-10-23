// Copyright Telephone Studios. All Rights Reserved.


#include "Items/ToolItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/EquipComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Actors/HarvestableResource.h"
#include "Components/HarvestableComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/EngineTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/DamageEvents.h"
#include "SurfaceHelpers.h"
#include "WildOmissionGameUserSettings.h"
#include "Log.h"

AToolItem::AToolItem()
{
	ToolType = EToolType::WOOD;

	GatherMultiplier = 1.0f;
	EffectiveRangeCentimeters = 150.0f;
	SwingSpeedRate = 1.0f;
	
	QualityTool = false;

	Durability = 100;

	SwingMontage = nullptr;
	SwingItemMontage = nullptr;

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

void AToolItem::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);

	UPlayerInventoryComponent* OwnerInventory = GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
	if (OwnerInventory == nullptr)
	{
		return;
	}

	FInventorySlot* FromSlot = OwnerInventory->GetSlot(InFromSlotIndex);
	if (FromSlot == nullptr)
	{
		return;
	}

	Durability = FromSlot->Item.GetStat(TEXT("Durability"));
}

void AToolItem::OnUnequip()
{
	Super::OnUnequip();

}

void AToolItem::OnPrimaryHeld()
{
	Super::OnPrimaryHeld();

	if (GetOwner() == nullptr || GetOwnerEquipComponent() == nullptr || GetOwnerEquipComponent()->IsMontagePlaying(SwingMontage))
	{
		return;
	}

	GetOwnerEquipComponent()->PlayItemMontage(SwingMontage, SwingItemMontage);
}

void AToolItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
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

	AHarvestableResource* HitHarvestable = Cast<AHarvestableResource>(HitResult.GetActor());
	UHarvestableComponent* HitHarvestableComponent = HitResult.GetActor()->FindComponentByClass<UHarvestableComponent>();
	APawn* HitPawn = Cast<APawn>(HitResult.GetActor());
	if (HitHarvestable != nullptr && (HitHarvestable->GetRequiredToolType() == ToolType || ToolType == EToolType::MULTI))
	{
		HitHarvestable->OnHarvest(GetOwner(), GatherMultiplier, QualityTool);
	}
	else if (HitHarvestableComponent != nullptr && (HitHarvestableComponent->GetRequiredToolType() == ToolType || ToolType == EToolType::MULTI))
	{
		HitHarvestableComponent->OnHarvest(GetOwner(), GatherMultiplier, QualityTool);
	}
	else if (HitPawn)
	{
		FPointDamageEvent HitByToolEvent(20.0f, HitResult, OwnerCharacterLookVector, nullptr);
		HitPawn->TakeDamage(20.0f, HitByToolEvent, GetOwnerPawn()->GetController(), this);
	}
	else
	{
		float DamageAmount = 5.0f;

		FPointDamageEvent HitByToolEvent(DamageAmount * GatherMultiplier, HitResult, OwnerCharacterLookVector, nullptr);
		HitResult.GetActor()->TakeDamage(DamageAmount * GatherMultiplier, HitByToolEvent, GetOwnerPawn()->GetController(), this);
	}

	UpdateDurability();
}

float AToolItem::GetGatherMultiplier() const
{
	return GatherMultiplier;
}

float AToolItem::GetSwingSpeedRate() const
{
	return SwingSpeedRate;
}

bool AToolItem::IsQualityTool() const
{
	return QualityTool;
}

void AToolItem::UpdateDurability()
{
	--Durability;

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

	FromSlot->Item.SetStat(TEXT("Durability"), Durability);
	OwnerInventory->RequestInventoryRefresh();
	
	if (Durability <= 0)
	{
		OwnerInventory->RemoveHeldItem();
	}
}

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

void AToolItem::PlayImpactSound(const FHitResult& HitResult)
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

void AToolItem::SpawnImpactParticles(const FHitResult& HitResult, const FVector& ImpactorForwardVector)
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

void AToolItem::SpawnImpactDecal(const FHitResult& HitResult)
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

void AToolItem::PlayCameraShake()
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
