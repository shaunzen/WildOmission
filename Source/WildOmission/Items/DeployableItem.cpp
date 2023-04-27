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
}

void ADeployableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PreviewActor)
	{
		bool SpawnValid = false;
		PreviewActor->SetActorTransform(GetPlacementTransform(SpawnValid));
		PreviewActor->Update(SpawnValid);
		bPrimaryEnabled = SpawnValid;
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
	bool SpawnValid = false;
	SpawnedDeployable->SetActorTransform(GetPlacementTransform(SpawnValid));

	if (SpawnValid == false)
	{
		SpawnedDeployable->Destroy();
	}

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
	bool InvalidOverlapFromPreview = false;
	if (HitActor == nullptr)
	{
		OutValidSpawn = false;
		return GetFreehandPlacementTransform();
	}

	if (PreviewActor)
	{
		InvalidOverlapFromPreview = PreviewActor->IsOverlappingInvalidObject();
	}

	// Check ground condition
	if (HitActor->ActorHasTag(FName("Ground")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnGround())
	{
		OutValidSpawn = !InvalidOverlapFromPreview;
		return GetFreehandPlacementTransform();
	}

	// Check floor condition
	if (HitActor->ActorHasTag(FName("Floor")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnFloor())
	{
		OutValidSpawn = !InvalidOverlapFromPreview;
		return GetFreehandPlacementTransform();
	}

	// Check wall condition
	if (HitActor->ActorHasTag(FName("Wall")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnWall())
	{
		OutValidSpawn = !InvalidOverlapFromPreview;
		return GetFreehandPlacementTransform();
	}

	// Check anchor condition
	if (UBuildAnchorComponent* HitBuildAnchor = Cast<UBuildAnchorComponent>(HitResult.GetComponent()))
	{
		UE_LOG(LogTemp, Warning, TEXT("hmm we seems to be looking at a build anchor??"));
		OutValidSpawn = HitBuildAnchor->GetType() == DeployableActorClass.GetDefaultObject()->CanSpawnOnBuildAnchor() && !InvalidOverlapFromPreview;
		if (OutValidSpawn == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("to bad our spawn condition wasnt valid."));
			return GetFreehandPlacementTransform();
		}
		return HitBuildAnchor->GetCorrectedTransform();
	}

	return GetFreehandPlacementTransform();
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