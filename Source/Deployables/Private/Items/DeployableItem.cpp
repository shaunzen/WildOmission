// Copyright Telephone Studios. All Rights Reserved.


#include "Items/DeployableItem.h"
#include "Deployables/Deployable.h"
#include "Actors/DeployablePreview.h"

void ADeployableItem::OnPlace()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	ADeployable* SpawnedDeployable = World->SpawnActor<ADeployable>(DeployableActorClass);
	if (SpawnedDeployable == nullptr)
	{
		return;
	}

	FTransform PlacementTransform;
	bool SpawnValid = GetPlacementTransform(PlacementTransform);
	SpawnedDeployable->SetActorTransform(PlacementTransform);
	SpawnedDeployable->OnSpawn();

	if (SpawnValid == false)
	{
		SpawnedDeployable->Destroy();
	}

	Super::OnPlace();
}

bool ADeployableItem::GetPlacementTransform(FTransform& OutPlacementTransform)
{
	Super::GetPlacementTransform(OutPlacementTransform);

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
			return false;
		}

		const bool ValidSpawn = HitBuildAnchor->GetType() == DeployableActorClass.GetDefaultObject()->CanSpawnOnBuildAnchor() && !InvalidOverlap;
		
		if (DeployableActorClass.GetDefaultObject()->CanRotate())
		{
			float FacePlayerYaw = GetFacePlayerRotation(HitBuildAnchor->GetComponentLocation()).Yaw;
			FacePlayerYaw = FMath::RoundToFloat(FacePlayerYaw / 90.0f) * 90.0f;
			
			float AnchorOffsetFromNearestSnap = GetOffsetFromNearestSnapDegree(AnchorHitResult.GetActor()->GetActorRotation().Yaw);
			
			FTransform BuildAnchorThatFacesPlayer = HitBuildAnchor->GetCorrectedTransform();
			BuildAnchorThatFacesPlayer.SetRotation(FQuat(FRotator(0.0f, FacePlayerYaw + AnchorOffsetFromNearestSnap, 0.0f)));
			
			OutPlacementTransform = BuildAnchorThatFacesPlayer;
			return HasAuthorizationToBuild(OutPlacementTransform.GetLocation());
		}

		OutPlacementTransform = HitBuildAnchor->GetCorrectedTransform();
		return HasAuthorizationToBuild(OutPlacementTransform.GetLocation());
	}

	FHitResult HitResult;
	if (!LineTraceOnChannel(ECC_Visibility, HitResult))
	{
		return false;
	}

	AActor* HitActor = HitResult.GetActor();
	
	if (HitActor == nullptr)
	{
		return false;
	}


	// Check ground condition
	if (HitActor->ActorHasTag(TEXT("Ground")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnGround())
	{
		return !InvalidOverlap && HasAuthorizationToBuild(HitResult.ImpactPoint);
	}

	// Check floor condition
	const bool NormalUpsideDown = HitResult.ImpactNormal.Z < 0.5f;
	const bool CanSpawnOnFloor = DeployableActorClass.GetDefaultObject()->CanSpawnOnFloor() && !NormalUpsideDown;
	const bool CanSpawnOnCeiling = DeployableActorClass.GetDefaultObject()->CanSpawnOnCeiling() && NormalUpsideDown;
	
	if (HitActor->ActorHasTag(TEXT("Floor")) && (CanSpawnOnFloor || CanSpawnOnCeiling))
	{
		return !InvalidOverlap && HasAuthorizationToBuild(HitResult.ImpactPoint);
	}

	// Check wall condition
	if (HitActor->ActorHasTag(TEXT("Wall")) && DeployableActorClass.GetDefaultObject()->CanSpawnOnWall())
	{
		return !InvalidOverlap && HasAuthorizationToBuild(HitResult.ImpactPoint);
	}

	return false;
}

float ADeployableItem::GetOffsetFromNearestSnapDegree(const float& InAxis) const
{
	float SnapTicks = InAxis / 90.0f;
	float WholeTicksFromNearestSnapDegree= FMath::RoundToFloat(SnapTicks);
	float OffsetPercentFromNearestSnapDegree = SnapTicks - WholeTicksFromNearestSnapDegree;

	return OffsetPercentFromNearestSnapDegree * 90.0f;
}