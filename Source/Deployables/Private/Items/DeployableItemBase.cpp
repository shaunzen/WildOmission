// Copyright Telephone Studios. All Rights Reserved.


#include "Items/DeployableItemBase.h"
#include "Deployables/Deployable.h"
#include "Actors/DeployablePreview.h"
#include "Components/PlayerInventoryComponent.h"
#include "Deployables/ToolCupboard.h"
#include "Components/BuilderComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

ADeployableItemBase::ADeployableItemBase()
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

void ADeployableItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PreviewActor)
	{
		FTransform PlacementTransform;
		bool SpawnValid = GetPlacementTransform(PlacementTransform);
		PreviewActor->SetActorTransform(PlacementTransform);
		PreviewActor->Update(SpawnValid);
		bPrimaryEnabled = SpawnValid;
	}
}

void ADeployableItemBase::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);

	Client_SpawnPreview();
}

void ADeployableItemBase::OnUnequip()
{
	Super::OnUnequip();

	Client_DestroyPreview();
}

void ADeployableItemBase::Destroyed()
{
	Super::Destroyed();
	
	if (PreviewActor)
	{
		PreviewActor->Destroy();
	}
}

void ADeployableItemBase::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();
	
	if (!HasAuthority())
	{
		return;
	}

	OnPlace();
}

void ADeployableItemBase::OnPlace()
{
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

bool ADeployableItemBase::LineTraceOnChannel(TEnumAsByte<ECollisionChannel> ChannelToTrace, FHitResult& OutHitResult) const
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

bool ADeployableItemBase::GetPlacementTransform(FTransform& OutPlacementTransform)
{
	OutPlacementTransform = GetFreehandPlacementTransform();
	return false;
}

FTransform ADeployableItemBase::GetFreehandPlacementTransform()
{
	FVector PlacementLocation = FVector::ZeroVector;
	FRotator PlacementRotation = FRotator::ZeroRotator;
	FVector PlacementUp = FVector::UpVector;

	FHitResult HitResult;
	if (LineTraceOnChannel(ECC_Visibility, HitResult))
	{
		PlacementLocation = HitResult.ImpactPoint;
		if (DeployableActorClass && DeployableActorClass.GetDefaultObject()->FollowsSurfaceNormal())
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

FRotator ADeployableItemBase::GetFacePlayerRotation(const FVector& PlacementLocation, const FVector& Up) const
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwnerPawn()->GetActorLocation(), PlacementLocation);

	FRotator PlacementUpRotationOffset = FRotator(Up.Rotation().Pitch - 90.0f, 0.0f, Up.Rotation().Roll - 90.0f);
	FVector PlacementForward = -UKismetMathLibrary::GetForwardVector(FRotator(0.0f, LookAtRotation.Yaw, 0.0f) - PlacementUpRotationOffset);
	FVector PlacementRight = -UKismetMathLibrary::GetRightVector(FRotator(0.0f, LookAtRotation.Yaw, 0.0f));


	return UKismetMathLibrary::MakeRotationFromAxes(PlacementForward, PlacementRight, Up);
}

UStaticMesh* ADeployableItemBase::GetPreviewMesh()
{
	return nullptr;
}

void ADeployableItemBase::Client_SpawnPreview_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	if (PreviewActor)
	{
		PreviewActor->Destroy();
		PreviewActor = nullptr;
	}

	PreviewActor = World->SpawnActor<ADeployablePreview>();
	if (DeployableActorClass.GetDefaultObject() != nullptr)
	{
		PreviewActor->Setup(DeployableActorClass.GetDefaultObject());
	}
	else if (GetPreviewMesh() != nullptr)
	{
		PreviewActor->Setup(GetPreviewMesh());
	}
}

void ADeployableItemBase::Client_DestroyPreview_Implementation()
{
	if (PreviewActor == nullptr)
	{
		return;
	}

	PreviewActor->Destroy();
	PreviewActor = nullptr;
}

bool ADeployableItemBase::HasAuthorizationToBuild(const FVector& LocationToTest) const
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return false;
	}

	UBuilderComponent* OwnerBuilderComponent = OwnerActor->FindComponentByClass<UBuilderComponent>();
	if (OwnerBuilderComponent == nullptr)
	{
		return false;
	}

	const FString& OwnerID = OwnerBuilderComponent->GetOwnerUniqueID();

	TArray<AToolCupboard*> AllToolCupboards = AToolCupboard::GetAllToolCupboards();
	for (AToolCupboard* ToolCupboard : AllToolCupboards)
	{
		if (ToolCupboard == nullptr || !ToolCupboard->IsWithinRange(LocationToTest) || ToolCupboard->IsPlayerAuthorized(OwnerID))
		{
			continue;
		}

		return false;
	}

	return true;
}
