// Copyright Telephone Studios. All Rights Reserved.


#include "Items/DeployableItem.h"
#include "Deployables/Deployable.h"
#include "Components/PlayerInventoryComponent.h"
#include "Actors/DeployablePreview.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

ADeployableItem::ADeployableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	DeployableActorClass = nullptr;
	DeployableRange = 500.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HammerMesh(TEXT("/Game/Deployables/Art/SK_Hammer"));
	if (HammerMesh.Succeeded())
	{
		MeshComponent->SetSkeletalMeshAsset(HammerMesh.Object);
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

void ADeployableItem::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);

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

void ADeployableItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

	if (!HasAuthority())
	{
		return;
	}
	
	UWorld* World = GetWorld();
	AActor* OwnerActor = GetOwner();
	if (World == nullptr || OwnerActor == nullptr)
	{
		return;
	}

	UPlayerInventoryComponent* OwnerInventoryComponent = OwnerActor->FindComponentByClass<UPlayerInventoryComponent>();
	if (OwnerInventoryComponent == nullptr)
	{
		return;
	}

	ADeployable* SpawnedDeployable = World->SpawnActor<ADeployable>(DeployableActorClass);
	if (SpawnedDeployable == nullptr)
	{
		return;
	}

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
	if (GetOwnerPawn() == nullptr)
	{
		return false;
	}

	FVector Start = GetOwnerPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	FVector End = Start + (UKismetMathLibrary::GetForwardVector(GetOwnerPawn()->GetControlRotation()) * DeployableRange);	// In this case the replicated rotation isnt needed,
	FCollisionQueryParams Params;																							// this is because the autonomys proxy and the authority are the only ones
																															// who will be reading this data, both are well aware of the control rotatation
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
			float FacePlayerYaw = GetFacePlayerRotation(HitBuildAnchor->GetComponentLocation()).Yaw;
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
	if (HitActor->ActorHasTag(TEXT("Ground")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnGround())
	{
		OutValidSpawn = !InvalidOverlap;
		return GetFreehandPlacementTransform();
	}

	// Check floor condition
	const bool NormalUpsideDown = HitResult.ImpactNormal.Z < 0.5f;
	const bool CanSpawnOnFloor = DeployableActorClass.GetDefaultObject()->CanSpawnOnFloor() && !NormalUpsideDown;
	const bool CanSpawnOnCeiling = DeployableActorClass.GetDefaultObject()->CanSpawnOnCeiling() && NormalUpsideDown;
	
	if (HitActor->ActorHasTag(TEXT("Floor")) && (CanSpawnOnFloor || CanSpawnOnCeiling))
	{
		OutValidSpawn = !InvalidOverlap;
		return GetFreehandPlacementTransform();
	}

	// Check wall condition
	if (HitActor->ActorHasTag(TEXT("Wall")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnWall())
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
		PlacementLocation = GetOwnerPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(GetOwnerPawn()->GetControlRotation()) * DeployableRange);
	}

	PlacementRotation = GetFacePlayerRotation(PlacementLocation, PlacementUp);

	FTransform PlacementTransform;
	PlacementTransform.SetLocation(PlacementLocation);
	PlacementTransform.SetRotation(PlacementRotation.Quaternion());

	return PlacementTransform;
}

FRotator ADeployableItem::GetFacePlayerRotation(const FVector& PlacementLocation, const FVector& Up) const
{
	FVector PlacementUp = Up;
	
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwnerPawn()->GetActorLocation(), PlacementLocation);
	
	FRotator PlacementUpRotationOffset = FRotator(PlacementUp.Rotation().Pitch - 90.0f, 0.0f, PlacementUp.Rotation().Roll - 90.0f);
	FVector PlacementForward = -UKismetMathLibrary::GetForwardVector(FRotator(0.0f, LookAtRotation.Yaw, 0.0f) - PlacementUpRotationOffset);
	FVector PlacementRight = -UKismetMathLibrary::GetRightVector(FRotator(0.0f, LookAtRotation.Yaw, 0.0f));

	
	return UKismetMathLibrary::MakeRotationFromAxes(PlacementForward, PlacementRight, PlacementUp);
}

float ADeployableItem::GetOffsetFromNearestSnapDegree(const float& InAxis) const
{
	// Add to this?
	float SnapTicks = InAxis / 90.0f;
	float WholeTicksFromNearestSnapDegree= FMath::RoundToFloat(SnapTicks);
	float OffsetPercentFromNearestSnapDegree = SnapTicks - WholeTicksFromNearestSnapDegree;

	return OffsetPercentFromNearestSnapDegree * 90.0f;
}