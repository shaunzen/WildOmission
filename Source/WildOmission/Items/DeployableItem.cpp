// Copyright Telephone Studios. All Rights Reserved.


#include "DeployableItem.h"
#include "Engine/StaticMeshActor.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

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
	OnDoorway = false;
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
	
	// TODO Validate spawn before spawning

	FTransform SpawnTransform = GetSpawnTransform();
	
	GetWorld()->SpawnActor<ADeployable>(DeployableActorClass, SpawnTransform);

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

FTransform ADeployableItem::GetSpawnTransform() const
{
	FTransform SpawnTransform;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FHitResult HitResult;
	if (!LineTraceOnCameraChannel(HitResult))
	{
		return SpawnTransform;
	}
	
	SpawnTransform.SetLocation(HitResult.ImpactPoint);
	if (DeployableActorClass.GetDefaultObject()->FollowsSurfaceNormal())
	{
		SpawnRotation = HitResult.ImpactNormal.Rotation();
	}
	SpawnRotation.Roll = -GetOwnerCharacter()->GetControlRotation().Yaw;
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	return SpawnTransform;
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

	FVector PreviewLocation = FVector::ZeroVector;
	FRotator PreviewRotation = FRotator::ZeroRotator;

	FHitResult HitResult;
	if (LineTraceOnCameraChannel(HitResult))
	{
		PreviewLocation = HitResult.ImpactPoint;
		if (DeployableActorClass.GetDefaultObject()->FollowsSurfaceNormal())
		{
			PreviewRotation = HitResult.ImpactNormal.Rotation();
		}
		PreviewRotation.Roll = -GetOwnerCharacter()->GetControlRotation().Yaw;
		WithinRange = true;
	}
	else
	{
		PreviewLocation = GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetControlRotation()) * DeployableRange);
		WithinRange = false;
	}

	PreviewActor->SetActorLocation(PreviewLocation);
	PreviewActor->SetActorRotation(PreviewRotation);
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
	case DoorwayOnly:
		return DoorwayOnlySpawnConditionValid();
		break;
	case AnyExceptInvalid:
		return AnyExceptInvalidSpawnConditionValid();
		break;
	case AnySurface:
		return AnySurfaceSpawnConditionValid();
		break;
	}

	return false;
}

bool ADeployableItem::GroundOnlySpawnConditionValid() const
{
	return OnGround == true && OnFloor == false && OnWall == false && OnDoorway == false && InvalidOverlap == false && WithinRange == true;
}

bool ADeployableItem::FloorOnlySpawnConditionValid() const
{
	return OnGround == false && OnFloor == true && OnWall == false && OnDoorway == false && InvalidOverlap == false && WithinRange == true;
}

bool ADeployableItem::GroundOrFloorSpawnConditionValid() const
{
	return (OnGround == true || OnFloor == true) && (OnWall == false && OnDoorway == false && InvalidOverlap == false && WithinRange == true);
}

bool ADeployableItem::WallOnlySpawnConditionValid() const
{
	return OnGround == false && OnFloor == false && OnWall == true && OnDoorway == false && InvalidOverlap == false && WithinRange == true;
}

bool ADeployableItem::DoorwayOnlySpawnConditionValid() const
{
	return OnGround == false && OnFloor == false && OnWall == false && OnDoorway == true && InvalidOverlap == false && WithinRange == true;
}

bool ADeployableItem::AnyExceptInvalidSpawnConditionValid() const
{
	return (OnGround == true || OnFloor == true || OnWall == true || OnDoorway == true) && InvalidOverlap == false && WithinRange == true;
}

bool ADeployableItem::AnySurfaceSpawnConditionValid() const
{
	return (OnGround == true || OnFloor == true || OnWall == true || OnDoorway == true || InvalidOverlap == true) && WithinRange == true;
}

void ADeployableItem::OnPreviewBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Overlap Other Actor: %s"), *OtherActor->GetActorNameOrLabel());
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
	else if (OtherActor->ActorHasTag(FName("Doorway")))
	{
		OnDoorway = true;
	}
	else
	{
		InvalidOverlap = true;
	}
}

void ADeployableItem::OnPreviewEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("End Overlap Other Actor: %s"), *OtherActor->GetActorNameOrLabel());
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
	else if (OtherActor->ActorHasTag(FName("Doorway")))
	{
		OnDoorway = false;
	}
	else
	{
		InvalidOverlap = false;
	}
}