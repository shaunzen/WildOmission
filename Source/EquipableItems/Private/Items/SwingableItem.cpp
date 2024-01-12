// Copyright Telephone Studios. All Rights Reserved.


#include "Items/SwingableItem.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "ChunkManager.h"
#include "Actors/Chunk.h"
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
	DealDamageToActors = true;
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
	if (OwnerEquipComponent == nullptr || OwnerEquipComponent->IsMontagePlaying(SwingMontage))
	{
		return;
	}

	OwnerEquipComponent->PlayItemMontage(SwingMontage, SwingItemMontage);
}

void ASwingableItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnPrimaryAnimationClimax(FromFirstPersonInstance);

	const UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	PlayCameraShake();

	const FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(OwnerEquipComponent->GetOwnerControlRotation());

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = true;

	const FVector Start = GetTraceStart();
	const FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

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

	if (DealDamageToActors)
	{
		AActor* HitActor = HitResult.GetActor();
		APawn* HitPawn = Cast<APawn>(HitActor);
		if (HitPawn)
		{
			float HeadshotDamage = 0.0f;
			if (HitResult.BoneName == TEXT("Head"))
			{
				HeadshotDamage = 25.0f;

				PlayHitmarkerSound(true);
			}
			else
			{
				PlayHitmarkerSound(false);
			}

			FPointDamageEvent HitByToolEvent((100.0f + HeadshotDamage) * DamageMultiplier, HitResult, OwnerCharacterLookVector, nullptr);
			HitPawn->TakeDamage((100.0f + HeadshotDamage) * DamageMultiplier, HitByToolEvent, GetOwnerPawn()->GetController(), this);
		}
		else if (HitActor)
		{
			float DamageAmount = 10.0f;

			FPointDamageEvent HitByToolEvent(DamageAmount * DamageMultiplier, HitResult, OwnerCharacterLookVector, nullptr);
			HitActor->TakeDamage(DamageAmount * DamageMultiplier, HitByToolEvent, GetOwnerPawn()->GetController(), this);
		}
	}

	DecrementDurability();
}

void ASwingableItem::DecrementDurability()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	UPlayerInventoryComponent* OwnerInventory = OwnerActor->FindComponentByClass<UPlayerInventoryComponent>();
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

	if (NewDurability <= 0)
	{
		OwnerInventory->RemoveHeldItem();
	}

	OwnerInventory->RequestInventoryRefresh();
}

void ASwingableItem::PlayImpactSound(const FHitResult& HitResult)
{
	// Declare decal material
	USoundBase* ImpactSound = nullptr;

	// Check if we hit the gound
	if (AChunk* HitChunk = Cast<AChunk>(HitResult.GetActor()))
	{
		// Use the surface type of the ground
		AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
		if (ChunkManager == nullptr)
		{
			return;
		}

		ImpactSound = USurfaceHelpers::GetImpactSound(ChunkManager->GetSurfaceTypeAtLocation(HitResult.ImpactPoint));
	}
	else // Use the physical material
	{
		// Check if the physical material is valid
		if (HitResult.PhysMaterial == nullptr)
		{
			return;
		}

		ImpactSound = USurfaceHelpers::GetImpactSound(HitResult.PhysMaterial->SurfaceType);

	}

	// Check if the sound is valid
	if (ImpactSound == nullptr)
	{
		return;
	}

	// Play sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);
}

void ASwingableItem::SpawnImpactParticles(const FHitResult& HitResult, const FVector& ImpactorForwardVector)
{
	// Declare particles
	UNiagaraSystem* ImpactParticles = nullptr;

	// Check if we hit the gound
	if (AChunk* HitChunk = Cast<AChunk>(HitResult.GetActor()))
	{
		// Use the surface type of the ground
		AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
		if (ChunkManager == nullptr)
		{
			return;
		}

		ImpactParticles = USurfaceHelpers::GetImpactParticles(ChunkManager->GetSurfaceTypeAtLocation(HitResult.ImpactPoint));
	}
	else // Use the physical material
	{
		// Check if the physical material is valid
		if (HitResult.PhysMaterial == nullptr)
		{
			return;
		}

		ImpactParticles = USurfaceHelpers::GetImpactParticles(HitResult.PhysMaterial->SurfaceType);

	}

	// Check if the particles are valid
	if (ImpactParticles == nullptr)
	{
		return;
	}

	// Spawn particles
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticles, HitResult.ImpactPoint, (-ImpactorForwardVector).Rotation());
}

void ASwingableItem::SpawnImpactDecal(const FHitResult& HitResult)
{
	// Declare decal material
	UMaterialInterface* DecalMaterial = nullptr;

	// Check if we hit the gound
	if (AChunk* HitChunk = Cast<AChunk>(HitResult.GetActor()))
	{
		// Use the surface type of the ground
		AChunkManager* ChunkManager = AChunkManager::GetChunkManager();
		if (ChunkManager == nullptr)
		{
			return;
		}

		DecalMaterial = USurfaceHelpers::GetImpactDecal(ChunkManager->GetSurfaceTypeAtLocation(HitResult.ImpactPoint));
	}
	else // Use the physical material
	{
		// Check if the physical material is valid
		if (HitResult.PhysMaterial == nullptr)
		{
			return;
		}

		DecalMaterial = USurfaceHelpers::GetImpactDecal(HitResult.PhysMaterial->SurfaceType);

	}
	
	// Check if the material is valid
	if (DecalMaterial == nullptr)
	{
		return;
	}

	// Spawn decal
	const FVector DecalSize = FVector(8.0f, 15.0f, 15.0f);
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

void ASwingableItem::PlayHitmarkerSound(bool Headshot)
{
	if (!IsOwnerOurPlayer())
	{
		return;
	}

	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	Headshot ? OwnerEquipComponent->Client_PlayHeadshotHitmarkerSound()
		: OwnerEquipComponent->Client_PlayHitmarkerSound();
}

FVector ASwingableItem::GetTraceStart() const
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn == nullptr)
	{
		return FVector::ZeroVector;
	}
	
	UCameraComponent* OwnerCameraComponent = OwnerPawn->FindComponentByClass<UCameraComponent>();
	if (OwnerCameraComponent)
	{
		return OwnerCameraComponent->GetComponentLocation();
	}

	return OwnerPawn->GetActorLocation();
}
