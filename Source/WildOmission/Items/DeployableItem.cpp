// Copyright Telephone Studios. All Rights Reserved.


#include "DeployableItem.h"
#include "Engine/StaticMeshActor.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "DrawDebugHelpers.h"

static UMaterialInstance* PreviewMaterial = nullptr;

ADeployableItem::ADeployableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	DeployableActorClass = nullptr;
	DeployableRange = 500.0f;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> PreviewMaterialInstanceBlueprint(TEXT("/Game/WildOmission/Art/Deployables/M_DeployablePreview_Inst"));
	if (PreviewMaterialInstanceBlueprint.Succeeded())
	{
		PreviewMaterial = PreviewMaterialInstanceBlueprint.Object;
	}

	OnGround = false;
	OnFloor = false;
	OnWall = false;
	InAnchor = false;
	InvalidOverlap = false;
}

void ADeployableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdatePreview();
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
	
	FTransform SpawnTransform = GetPlacementTransform();
	
	ADeployable* SpawnedDeployable = GetWorld()->SpawnActor<ADeployable>(DeployableActorClass, SpawnTransform);

	OwnerInventoryComponent->RemoveHeldItem();
}

bool ADeployableItem::LineTraceOnCameraChannel(FHitResult& OutHitResult) const
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

	if (GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Camera, Params))
	{
		return true;
	}

	return false;
}

FTransform ADeployableItem::GetPlacementTransform()
{
	if (DeployableActorClass.GetDefaultObject()->SnapsToBuildAnchor() != EBuildAnchorType::None)
	{
		return GetSnappingPlacementTransform();
	}
	else
	{
		return GetNonSnappingPlacementTransform();
	}
}

FTransform ADeployableItem::GetSnappingPlacementTransform()
{
	return FTransform();
}

FTransform ADeployableItem::GetNonSnappingPlacementTransform()
{
	FVector PlacementLocation = FVector::ZeroVector;
	FRotator PlacementRotation = FRotator::ZeroRotator;
	FVector PlacementForward = -UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetOwnerCharacter()->GetControlRotation().Yaw, 0.0f));
	FVector PlacementRight = -UKismetMathLibrary::GetRightVector(GetOwnerCharacter()->GetControlRotation());
	FVector PlacementUp = FVector(0.0f, 0.0f, 1.0f);

	FHitResult HitResult;
	if (LineTraceOnCameraChannel(HitResult))
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

void ADeployableItem::Client_SpawnPreview_Implementation()
{
	if (DeployableActorClass == nullptr)
	{
		return;
	}

	UStaticMesh* DeployedActorMesh = DeployableActorClass.GetDefaultObject()->GetMesh();
	if (Mesh == nullptr)
	{
		return;
	}
	
	if (PreviewActor)
	{
		PreviewActor->Destroy();
	}

	PreviewActor = GetWorld()->SpawnActor<AStaticMeshActor>(GetOwner()->GetActorLocation() = FVector(0.0f, 0.0f, 500.0f), GetOwner()->GetActorRotation());
	PreviewActor->SetMobility(EComponentMobility::Movable);
	PreviewActor->GetStaticMeshComponent()->SetCollisionProfileName(FName("OverlapAll"));
	PreviewActor->GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	PreviewActor->GetStaticMeshComponent()->SetStaticMesh(DeployedActorMesh);
	PreviewActor->GetStaticMeshComponent()->SetMaterial(0, PreviewMaterial);
	PreviewActor->OnActorBeginOverlap.AddDynamic(this, &ADeployableItem::OnPreviewBeginOverlap);
	PreviewActor->OnActorEndOverlap.AddDynamic(this, &ADeployableItem::OnPreviewEndOverlap);
}

void ADeployableItem::Client_DestroyPreview_Implementation()
{
	if (PreviewActor == nullptr)
	{
		return;
	}

	PreviewActor->Destroy();
}

void ADeployableItem::UpdatePreview()
{
	if (PreviewActor == nullptr)
	{
		return;
	}

	PreviewActor->SetActorTransform(GetPlacementTransform());
	bPrimaryEnabled = SpawnConditionValid();
	PreviewActor->GetStaticMeshComponent()->SetScalarParameterValueOnMaterials(FName("Valid"), SpawnConditionValid());
}


bool ADeployableItem::SpawnConditionValid() const
{
	ADeployable* DefaultDeployable = DeployableActorClass.GetDefaultObject();

	switch (DefaultDeployable->GetPlacementType())
	{
	case GroundOnly:
		return GroundOnlySpawnConditionValid();
		break;
	case FloorOnly:
		return FloorOnlySpawnConditionValid();
		break;
	case GroundOrFloor:
		return GroundOrFloorSpawnConditionValid();
		break;
	case WallOnly:
		return WallOnlySpawnConditionValid();
		break;
	case AnySurface:
		return AnySurfaceSpawnConditionValid();
		break;
	}

	return false;
}

bool ADeployableItem::GroundOnlySpawnConditionValid() const
{
	return OnGround == true && OnFloor == false && OnWall == false && InAnchor == false && InvalidOverlap == false && WithinRange == true;
}

bool ADeployableItem::FloorOnlySpawnConditionValid() const
{
	return OnGround == false && OnFloor == true && OnWall == false && InAnchor == false && InvalidOverlap == false && WithinRange == true;
}

bool ADeployableItem::GroundOrFloorSpawnConditionValid() const
{
	return (OnGround == true || OnFloor == true) && (OnWall == false && InvalidOverlap == false && WithinRange == true);
}

bool ADeployableItem::WallOnlySpawnConditionValid() const
{
	return OnGround == false && OnFloor == false && OnWall == true && InvalidOverlap == false && WithinRange == true;
}

bool ADeployableItem::AnchorSpawnConditionValid() const
{
	return InAnchor == true && InvalidOverlap == false && WithinRange == true;
}

bool ADeployableItem::AnySurfaceSpawnConditionValid() const
{
	return (OnGround == true || OnFloor == true || OnWall == true || InvalidOverlap == true) && WithinRange == true;
}

void ADeployableItem::OnPreviewBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Ground")))
	{
		OnGround = true;
	}
	else if (OtherActor->ActorHasTag(FName("Floor")))
	{
		OnFloor = true;
	}
	else if (OtherActor->ActorHasTag(FName("Wall")))
	{
		OnWall = true;
	}
	else
	{
		InvalidOverlap = true;
	}
}

void ADeployableItem::OnPreviewEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Ground")))
	{
		OnGround = false;
	}
	else if (OtherActor->ActorHasTag(FName("Floor")))
	{
		OnFloor = false;
	}
	else if (OtherActor->ActorHasTag(FName("Wall")))
	{
		OnWall = false;
	}
	else
	{
		InvalidOverlap = false;
	}
}