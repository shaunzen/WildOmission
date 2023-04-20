// Copyright Telephone Studios. All Rights Reserved.


#include "DeployableItem.h"
#include "Engine/StaticMeshActor.h"
#include "WildOmission/Deployables/Deployable.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

ADeployableItem::ADeployableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	DeployableRange = 500.0f;
	SpawnConditionValid = false;
	
	ConstructorHelpers::FObjectFinder<UMaterialInstance> PreviewMaterialInstanceBlueprint(TEXT("/Game/WildOmission/Art/Deployables/M_DeployablePreview_Inst"));
	if (PreviewMaterialInstanceBlueprint.Succeeded() == false)
	{
		return;
	}

	PreviewMaterial = PreviewMaterialInstanceBlueprint.Object;
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

	// spawn on the server the real deployable item
	// remove this current item from our inventory
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
	PreviewActor->SetActorEnableCollision(false);
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

	FVector PreviewLocation;
	FHitResult HitResult;
	if (LineTraceOnCameraChannel(HitResult))
	{
		PreviewLocation = HitResult.ImpactPoint;
	}
	else
	{
		PreviewLocation = GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetControlRotation()) * DeployableRange);
	}
	PreviewActor->SetActorLocation(PreviewLocation);

	PreviewActor->GetStaticMeshComponent()->SetScalarParameterValueOnMaterials(FName("CanSpawn"), SpawnConditionValid);
}

void ADeployableItem::OnPreviewBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// TODO check spawn condition
}

void ADeployableItem::OnPreviewEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// TODO check spawn condition
}