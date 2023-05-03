// Copyright Telephone Studios. All Rights Reserved.


#include "DeployableItem.h"
#include "Engine/StaticMeshActor.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Deployables/DeployablePreview.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

ADeployableItem::ADeployableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	DeployableActorClass = nullptr;
	DeployableRange = 500.0f;

	ConstructorHelpers::FObjectFinder<UStaticMesh> HammerStaticMesh(TEXT("/Game/WildOmission/Art/Items/SM_Hammer"));
	if (HammerStaticMesh.Succeeded())
	{
		Mesh->SetStaticMesh(HammerStaticMesh.Object);
	}
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

void ADeployableItem::Destroyed()
{
	Super::Destroyed();

	if (PreviewActor)
	{
		PreviewActor->Destroy();
	}
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
	SpawnedDeployable->OnSpawn();

	if (SpawnValid == false)
	{
		SpawnedDeployable->Destroy();
	}

	OwnerInventoryComponent->RemoveHeldItem();
}

bool ADeployableItem::LineTraceOnChannel(TEnumAsByte<ECollisionChannel> ChannelToTrace, FHitResult& OutHitResult) const
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
	
	if (GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ChannelToTrace, Params))
	{
		return true;
	}

	return false;
}

FTransform ADeployableItem::GetPlacementTransform(bool& OutValidSpawn)
{
	bool InvalidOverlap = false;
	if (PreviewActor)
	{
		InvalidOverlap = PreviewActor->IsOverlappingInvalidObject();
	}

	// Check anchor condition
	FHitResult AnchorHitResult;
	bool HitAnchor = false;
	switch (DeployableActorClass.GetDefaultObject()->CanSpawnOnBuildAnchor())
	{
	case None:
		break;
	case FoundationAnchor:
		HitAnchor = LineTraceOnChannel(ECC_GameTraceChannel4, AnchorHitResult);
		break;
	case WallAnchor:
		HitAnchor = LineTraceOnChannel(ECC_GameTraceChannel5, AnchorHitResult);
		break;
	case FloorAnchor:
		HitAnchor = LineTraceOnChannel(ECC_GameTraceChannel6, AnchorHitResult);
		break;
	case DoorAnchor:
		HitAnchor = LineTraceOnChannel(ECC_GameTraceChannel7, AnchorHitResult);
		break;
	case HouseStairsAnchor:
		HitAnchor = LineTraceOnChannel(ECC_GameTraceChannel8, AnchorHitResult);
		break;
	}

	if (HitAnchor == true)
	{
		UBuildAnchorComponent* HitBuildAnchor = Cast<UBuildAnchorComponent>(AnchorHitResult.GetComponent());
		if (HitBuildAnchor == nullptr)
		{
			OutValidSpawn = false;
			return GetFreehandPlacementTransform();
		}

		OutValidSpawn = HitBuildAnchor->GetType() == DeployableActorClass.GetDefaultObject()->CanSpawnOnBuildAnchor() && !InvalidOverlap;
		
		if (DeployableActorClass.GetDefaultObject()->CanRotate())
		{
			float FacePlayerYaw = GetFacePlayerRotation().Yaw;
			FacePlayerYaw = FMath::RoundToFloat(FacePlayerYaw / 90.0f) * 90.0f;
			float AnchorOffsetFromNearestSnap = GetOffsetFromNearestSnapDegree(AnchorHitResult.GetActor()->GetActorRotation().Yaw);
			
			FTransform BuildAnchorThatFacesPlayer = HitBuildAnchor->GetCorrectedTransform();
			BuildAnchorThatFacesPlayer.SetRotation(FQuat(FRotator(0.0f, FacePlayerYaw + AnchorOffsetFromNearestSnap, 0.0f)));
			
			return BuildAnchorThatFacesPlayer;
		}

		return HitBuildAnchor->GetCorrectedTransform();
	}

	FHitResult HitResult;
	if (!LineTraceOnChannel(ECC_Visibility, HitResult))
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


	// Check ground condition
	if (HitActor->ActorHasTag(FName("Ground")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnGround())
	{
		OutValidSpawn = !InvalidOverlap;
		return GetFreehandPlacementTransform();
	}

	// Check floor condition
	if (HitActor->ActorHasTag(FName("Floor")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnFloor())
	{
		OutValidSpawn = !InvalidOverlap;
		return GetFreehandPlacementTransform();
	}

	// Check wall condition
	if (HitActor->ActorHasTag(FName("Wall")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnWall())
	{
		OutValidSpawn = !InvalidOverlap;
		return GetFreehandPlacementTransform();
	}

	OutValidSpawn = false;
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
	FVector PlacementUp = FVector::UpVector;

	FHitResult HitResult;
	if (LineTraceOnChannel(ECC_Visibility, HitResult))
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

	PlacementRotation = GetFacePlayerRotation(PlacementUp);

	FTransform PlacementTransform;
	PlacementTransform.SetLocation(PlacementLocation);
	PlacementTransform.SetRotation(PlacementRotation.Quaternion());

	return PlacementTransform;
}

FRotator ADeployableItem::GetFacePlayerRotation(const FVector& Up) const
{
	FVector PlacementForward = -UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetOwnerCharacter()->GetControlRotation().Yaw, 0.0f));
	FVector PlacementRight = -UKismetMathLibrary::GetRightVector(GetOwnerCharacter()->GetControlRotation());
	FVector PlacementUp = Up;
	
	return UKismetMathLibrary::MakeRotationFromAxes(PlacementForward, PlacementRight, PlacementUp);
}

float ADeployableItem::GetOffsetFromNearestSnapDegree(const float& InAxis) const
{
	float SnapTicks = InAxis / 90.0f;
	float WholeTicksFromNearestSnapDegree= FMath::Floor(SnapTicks);
	float OffsetPercentFromNearestSnapDegree = SnapTicks - WholeTicksFromNearestSnapDegree;

	return OffsetPercentFromNearestSnapDegree * 90.0f;
}