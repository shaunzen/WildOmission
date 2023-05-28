// Copyright Telephone Studios. All Rights Reserved.


#include "WindSuckerComponent.h"
#include "GameFramework/MovementComponent.h"

UWindSuckerComponent::UWindSuckerComponent()
{
	ForceStrength = -999999.0f;
	Radius = 3000.0f;
	Falloff = ERadialImpulseFalloff::RIF_Linear;
}

void UWindSuckerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsActive())
	{
		const FVector Origin = GetComponentLocation();

		// Find objects within the sphere
		TArray<FOverlapResult> Overlaps;

		FCollisionQueryParams Params(SCENE_QUERY_STAT(AddForceOverlap), false);

		// Ignore owner actor if desired
		if (bIgnoreOwningActor)
		{
			Params.AddIgnoredActor(GetOwner());
		}

		GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere(Radius), Params);

		// A component can have multiple physics presences (e.g. destructible mesh components).
		// The component should handle the radial force for all of the physics objects it contains
		// so here we grab all of the unique components to avoid applying impulses more than once.
		TArray<UPrimitiveComponent*, TInlineAllocator<1>> AffectedComponents;
		AffectedComponents.Reserve(Overlaps.Num());

		for (FOverlapResult& OverlapResult : Overlaps)
		{
			if (UPrimitiveComponent* PrimitiveComponent = OverlapResult.Component.Get())
			{
				AffectedComponents.AddUnique(PrimitiveComponent);
			}
		}

		for (UPrimitiveComponent* PrimitiveComponent : AffectedComponents)
		{
			if (!HasLineOfSightToComponent(PrimitiveComponent))
			{
				continue;
			}

			PrimitiveComponent->AddRadialForce(Origin, Radius, ForceStrength, Falloff);

			// see if this is a target for a movement component
			AActor* ComponentOwner = PrimitiveComponent->GetOwner();
			if (ComponentOwner)
			{
				TInlineComponentArray<UMovementComponent*> MovementComponents;
				ComponentOwner->GetComponents(MovementComponents);
				for (const auto& MovementComponent : MovementComponents)
				{
					if (MovementComponent->UpdatedComponent == PrimitiveComponent)
					{
						MovementComponent->AddRadialForce(Origin, Radius, ForceStrength, Falloff);
						break;
					}
				}
			}
		}
	}
}

bool UWindSuckerComponent::HasLineOfSightToComponent(UPrimitiveComponent* InComponent) const
{
	FVector TowardComponentVector = (GetComponentLocation() - InComponent->GetComponentLocation()).GetSafeNormal();
	float DistanceFromComponent = FVector::Distance(GetComponentLocation(), InComponent->GetComponentLocation());


	FHitResult HitResult;
	FVector Start = GetComponentLocation();
	FVector End = Start + (TowardComponentVector * DistanceFromComponent);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this->GetOwner());

	if (!GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(16.0f), Params))
	{
		return false;
	}

	if (HitResult.GetComponent() == nullptr || InComponent == nullptr)
	{
		return false;
	}

	return HitResult.GetComponent() == InComponent;
}