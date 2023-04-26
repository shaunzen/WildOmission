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

FTransform ADeployableItem::GetPlacementTransform()
{
	if (DeployableActorClass.GetDefaultObject()->GetPlacementType() == GroundOnly)
	{
		return GetFreehandPlacementTransform();
	}
	else
	{
		FHitResult HitResult;
		if (!LineTraceOnDeployableChannel(HitResult))
		{
			return GetFreehandPlacementTransform();
		}

		UBuildAnchorComponent* HitAnchor = Cast<UBuildAnchorComponent>(HitResult.GetComponent());
		if (HitAnchor == nullptr)
		{
			return GetFreehandPlacementTransform();
		}
		if (HitAnchor->GetType() != DeployableActorClass.GetDefaultObject()->SnapsToBuildAnchor())
		{
			return GetFreehandPlacementTransform();
		}

		WithinRange = true;
		InAnchor = true;
		return HitAnchor->GetComponentTransform();
	}
	return FTransform();

}

bool ADeployableItem::IsSpawnConditionValid() const
{
	UE_LOG(LogTemp, Warning, TEXT("IsGrounded %i, IsOverlappingInvalidObject %i, InAnchor %i, WithinRange %i"), PreviewActor->IsGrounded(), PreviewActor->IsOverlappingInvalidObject(), InAnchor, WithinRange);
	switch (DeployableActorClass.GetDefaultObject()->GetPlacementType())
	{
	case GroundOnly:
		return PreviewActor->IsGrounded() && !PreviewActor->IsOverlappingInvalidObject() && WithinRange;
		break;
	case AnchorOnly:
		return InAnchor && !PreviewActor->IsOverlappingInvalidObject() && WithinRange;
		break;
	case GroundOrAnchor:
		return (PreviewActor->IsGrounded()) && !PreviewActor->IsOverlappingInvalidObject() && WithinRange;
	}

	return false;
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
	InAnchor = false;
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

		WithinRange = true;
	}
	else
	{
		PlacementLocation = GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetControlRotation()) * DeployableRange);
		WithinRange = false;
	}

	PlacementRotation = UKismetMathLibrary::MakeRotationFromAxes(PlacementForward, PlacementRight, PlacementUp);

	FTransform PlacementTransform;
	PlacementTransform.SetLocation(PlacementLocation);
	PlacementTransform.SetRotation(PlacementRotation.Quaternion());

	return PlacementTransform;
}