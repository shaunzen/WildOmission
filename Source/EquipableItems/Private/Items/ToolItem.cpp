// Copyright Telephone Studios. All Rights Reserved.


#include "Items/ToolItem.h"
#include "Components/EquipComponent.h"
#include "Actors/HarvestableResource.h"
#include "Components/HarvestableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Log.h"

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

float AToolItem::GetGatherMultiplier() const
{
	return GatherMultiplier;
}

bool AToolItem::IsQualityTool() const
{
	return QualityTool;
}

void AToolItem::OnSwingImpact(const FHitResult& HitResult, const FVector& OwnerCharacterLookVector, bool FromFirstPersonInstance)
{
	Super::OnSwingImpact(HitResult, OwnerCharacterLookVector, FromFirstPersonInstance);

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
