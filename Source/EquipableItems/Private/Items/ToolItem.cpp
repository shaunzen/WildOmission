// Copyright Telephone Studios. All Rights Reserved.


#include "Items/ToolItem.h"
#include "Components/EquipComponent.h"
#include "Actors/HarvestableResource.h"
#include "Components/HarvestableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

AToolItem::AToolItem()
{
	ToolType = EToolType::WOOD;
	QualityTool = false;
}

void AToolItem::OnPrimaryHeld()
{
	Super::OnPrimaryHeld();

	Swing();
}

void AToolItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnPrimaryAnimationClimax(FromFirstPersonInstance);

	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(OwnerEquipComponent->GetOwnerControlRotation());

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = true;

	FVector Start = GetOwnerPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		UE_LOG(LogEquipableItems, Verbose, TEXT("Nothing was hit by tool."));
		return;
	}

	if (!HasAuthority())
	{
		return;
	}

	AHarvestableResource* HitHarvestable = Cast<AHarvestableResource>(HitResult.GetActor());
	UHarvestableComponent* HitHarvestableComponent = HitResult.GetActor()->FindComponentByClass<UHarvestableComponent>();
	APawn* HitPawn = Cast<APawn>(HitResult.GetActor());
	if (HitHarvestable != nullptr && (HitHarvestable->GetRequiredToolType() == ToolType || ToolType == EToolType::MULTI))
	{
		HitHarvestable->OnHarvest(GetOwner(), GatherMultiplier, QualityTool);
	}
	else if (HitHarvestableComponent != nullptr && (HitHarvestableComponent->GetRequiredToolType() == ToolType || ToolType == EToolType::MULTI))
	{
		HitHarvestableComponent->OnHarvest(GetOwner(), GatherMultiplier, QualityTool);
	}
}

float AToolItem::GetGatherMultiplier() const
{
	return GatherMultiplier;
}

bool AToolItem::IsQualityTool() const
{
	return QualityTool;
}
