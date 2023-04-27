// Copyright Telephone Studios. All Rights Reserved.


#include "DeployableItem.h"
#include "Engine/StaticMeshActor.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Deployables/DeployablePreview.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

ADeployableItem::ADeployableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	DeployableActorClass = nullptr;
	DeployableRange = 500.0f;

	InAnchor = false;
	WithinRange = false;
}

void ADeployableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PreviewActor)
	{
		PreviewActor->SetActorTransform(GetPlacementTransform());
		PreviewActor->Update(IsSpawnConditionValid());
		bPrimaryEnabled = IsSpawnConditionValid();
	}
}

void ADeployableItem::Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerCharacter, InItemName, InFromSlotIndex, InUniqueID);
	
	Client_SpawnPreview();
}

void ADeployableItem::OnUnequip()
{
	Super::OnUnequip();

	Client_DestroyPreview();
}

void ADeployableItem::Primary()
{
	Super::Primary();
	
	UPlayerInventoryComponent* OwnerInventoryComponent = GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
	if (OwnerInventoryComponent == nullptr)
	{
		return;
	}
	
	ADeployable* SpawnedDeployable = GetWorld()->SpawnActor<ADeployable>(DeployableActorClass);
	SpawnedDeployable->SetActorTransform(GetPlacementTransform());

	OwnerInventoryComponent->RemoveHeldItem();
}

bool ADeployableItem::LineTraceOnDeployableChannel(FHitResult& OutHitResult) const
{
	if (GetOwnerCharacter() == nullptr)
	{
		return false;
	}

	FVector Start = GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + (UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetControlRotation()) * DeployableRange);
	FCollisionQueryParams Params;

	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(this);
	if (PreviewActor)
	{
		Params.AddIgnoredActor(PreviewActor);
	}

	if (GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel4, Params))
	{
		return true;
	}

	return false;
}

FTransform ADeployableItem::GetPlacementTransform(bool& OutValidSpawn)
{
	FHitResult HitResult;
	if (!LineTraceOnDeployableChannel(HitResult))
	{
		OutValidSpawn = false;
		return GetFreehandPlacementTransform();
	}
	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr)
	{
		OutValidSpawn = false;
		return GetFreehandPlacementTransform();
	}

	// check ground condition
	if (HitActor->ActorHasTag(FName("Ground")))
	{
		OutValidSpawn = DeployableActorClass.GetDefaultObject()->CanSpawnOnGround() && !PreviewActor->IsOverlappingInvalidObject();
		return GetFreehandPlacementTransform();
	}

	// check floor condition
	if (HitActor->ActorHasTag(FName("Floor")))
	{
		OutValidSpawn = DeployableActorClass.GetDefaultObject()->CanSpawnOnFloor()  && !PreviewActor->IsOverlappingInvalidObject();
		return GetFreehandPlacementTransform();
	}

	// check wall condition
	if (HitActor->ActorHasTag(FName("Wall")))
	{
		OutValidSpawn = DeployableActorClass.GetDefaultObject()->CanSpawnOnWall() && !PreviewActor->IsOverlappingInvalidObject();
		return GetFreehandPlacementTransform();
	}

	if (UBuildAnchorComponent* HitBuildAnchor = Cast<UBuildAnchorComponent>(HitResult.GetComponent()))
	{
		OutValidSpawn = HitBuildAnchor->GetType() == DeployableActorClass.GetDefaultObject()->CanSpawnOnBuildAnchor() && !PreviewActor->IsOverlappingInvalidObject();
		if (OutValidSpawn == false)
		{
			return GetFreehandPlacementTransform();
		}
		return HitBuildAnchor->GetComponentTransform();
	}
}

void ADeployableItem::Client_SpawnPreview_Implementation()
{
	if (DeployableActorClass == nullptr)
	{
		return;
	}
	
	if (PreviewActor)
	{
		PreviewActor->Destroy();
	}

	PreviewActor = GetWorld()->SpawnActor<ADeployablePreview>(GetOwner()->GetActorLocation() = FVector(0.0f, 0.0f, 500.0f), GetOwner()->GetActorRotation());
	PreviewActor->Setup(DeployableActorClass.GetDefaultObject());
}

void ADeployableItem::Client_DestroyPreview_Implementation()
{
	if (PreviewActor == nullptr)
	{
		return;
	}

	PreviewActor->Destroy();
}

FTransform ADeployableItem::GetFreehandPlacementTransform()
{
	FVector PlacementLocation = FVector::ZeroVector;
	FRotator PlacementRotation = FRotator::ZeroRotator;
	FVector PlacementForward = -UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetOwnerCharacter()->GetControlRotation().Yaw, 0.0f));
	FVector PlacementRight = -UKismetMathLibrary::GetRightVector(GetOwnerCharacter()->GetControlRotation());
	FVector PlacementUp = FVector(0.0f, 0.0f, 1.0f);

	FHitResult HitResult;
	if (LineTraceOnDeployableChannel(HitResult))
	{
		PlacementLocation = HitResult.ImpactPoint;
		if (DeployableActorClass.GetDefaultObject()->FollowsSurfaceNormal())
		{
			PlacementUp = HitResult.ImpactNormal;
		}
	}
	else
	{
		PlacementLocation = GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetControlRotation()) * DeployableRange);
	}

	PlacementRotation = UKismetMathLibrary::MakeRotationFromAxes(PlacementForward, PlacementRight, PlacementUp);

	FTransform PlacementTransform;
	PlacementTransform.SetLocation(PlacementLocation);
	PlacementTransform.SetRotation(PlacementRotation.Quaternion());

	return PlacementTransform;
}