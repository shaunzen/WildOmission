// Copyright Telephone Studios. All Rights Reserved.


#include "Items/LockApplicationItem.h"
#include "Components/LockComponent.h"
#include "Locks/Lock.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "UObject/ConstructorHelpers.h"

ALockApplicationItem::ALockApplicationItem()
{
	PrimaryActorTick.bCanEverTick = true;

	LockActorClass = nullptr;
	PlacementRange = 500.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HammerMesh(TEXT("/Game/Deployables/Art/SK_Hammer"));
	if (HammerMesh.Succeeded())
	{
		MeshComponent->SetSkeletalMeshAsset(HammerMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> PreviewMaterialInstanceBlueprint(TEXT("/Game/Deployables/Art/M_DeployablePreview_Inst"));
	if (PreviewMaterialInstanceBlueprint.Succeeded())
	{
		PreviewMaterial = PreviewMaterialInstanceBlueprint.Object;
	}
}

void ALockApplicationItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PreviewActor)
	{
		FTransform PlacementTransform;
		bool SpawnValid = GetPlacementTransform(PlacementTransform);
		PreviewActor->SetActorTransform(PlacementTransform);
		bPrimaryEnabled = SpawnValid;

		UStaticMeshComponent* PreviewMeshComponent = PreviewActor->GetStaticMeshComponent();
		if (PreviewMeshComponent == nullptr)
		{
			return;
		}

		PreviewMeshComponent->SetScalarParameterValueOnMaterials(TEXT("Valid"), SpawnValid);
	}
}

void ALockApplicationItem::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);

	Client_SpawnPreview();
}

void ALockApplicationItem::OnUnequip()
{
	Super::OnUnequip();

	Client_DestroyPreview();
}

void ALockApplicationItem::Destroyed()
{
	Super::Destroyed();

	if (PreviewActor)
	{
		PreviewActor->Destroy();
	}
}

void ALockApplicationItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();
	
	// TODO handle placement logic

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

	OwnerInventory->RemoveHeldItem();
}

bool ALockApplicationItem::LineTraceOnChannel(TEnumAsByte<ECollisionChannel> ChannelToTrace, FHitResult& OutHitResult) const
{
	if (GetOwnerPawn() == nullptr)
	{
		return false;
	}

	FVector Start = GetOwnerPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	FVector End = Start + (UKismetMathLibrary::GetForwardVector(GetOwnerPawn()->GetControlRotation()) * PlacementRange);	// In this case the replicated rotation isnt needed,
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

bool ALockApplicationItem::GetPlacementTransform(FTransform& OutPlacementTransform)
{
	OutPlacementTransform = GetFreehandPlacementTransform();
	return false;
	// TODO handle transform logic
}

FTransform ALockApplicationItem::GetFreehandPlacementTransform()
{
	FVector PlacementLocation = FVector::ZeroVector;
	FRotator PlacementRotation = FRotator::ZeroRotator;
	FVector PlacementUp = FVector::UpVector;

	FHitResult HitResult;
	if (LineTraceOnChannel(ECC_Visibility, HitResult))
	{
		PlacementLocation = HitResult.ImpactPoint;
	}
	else
	{
		PlacementLocation = GetOwnerPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation() + (UKismetMathLibrary::GetForwardVector(GetOwnerPawn()->GetControlRotation()) * PlacementRange);
	}

	PlacementRotation = GetFacePlayerRotation(PlacementLocation, PlacementUp);

	FTransform PlacementTransform;
	PlacementTransform.SetLocation(PlacementLocation);
	PlacementTransform.SetRotation(PlacementRotation.Quaternion());

	return PlacementTransform;
}

FRotator ALockApplicationItem::GetFacePlayerRotation(const FVector& PlacementLocation, const FVector& Up) const
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwnerPawn()->GetActorLocation(), PlacementLocation);

	FRotator PlacementUpRotationOffset = FRotator(Up.Rotation().Pitch - 90.0f, 0.0f, Up.Rotation().Roll - 90.0f);
	FVector PlacementForward = -UKismetMathLibrary::GetForwardVector(FRotator(0.0f, LookAtRotation.Yaw, 0.0f) - PlacementUpRotationOffset);
	FVector PlacementRight = -UKismetMathLibrary::GetRightVector(FRotator(0.0f, LookAtRotation.Yaw, 0.0f));


	return UKismetMathLibrary::MakeRotationFromAxes(PlacementForward, PlacementRight, Up);
}

void ALockApplicationItem::Client_SpawnPreview_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr || LockActorClass == nullptr)
	{
		return;
	}

	if (PreviewActor)
	{
		PreviewActor->Destroy();
		PreviewActor = nullptr;
	}

	PreviewActor = World->SpawnActor<AStaticMeshActor>();
	if (PreviewActor == nullptr)
	{
		return;
	}

	UStaticMeshComponent* PreviewMesh = PreviewActor->GetStaticMeshComponent();
	if (PreviewMesh == nullptr)
	{
		return;
	}

	PreviewActor->SetMobility(EComponentMobility::Movable);
	PreviewMesh->SetCollisionProfileName(TEXT("NoCollision"));
	PreviewMesh->SetStaticMesh(LockActorClass.GetDefaultObject()->GetStaticMesh());

	// Set all materials on the mesh to use the preview material
	for (int32 i = 0; i < PreviewMesh->GetNumMaterials(); i++)
	{
		PreviewMesh->SetMaterial(i, PreviewMaterial);
	}
}

void ALockApplicationItem::Client_DestroyPreview_Implementation()
{
	if (PreviewActor == nullptr)
	{
		return;
	}
	PreviewActor->Destroy();
	PreviewActor = nullptr;
}
