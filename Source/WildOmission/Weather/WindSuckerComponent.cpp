// Copyright Telephone Studios. All Rights Reserved.


#include "WindSuckerComponent.h"
#include "GameFramework/MovementComponent.h"

// Sets default values for this component's properties
UWindSuckerComponent::UWindSuckerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Radius = 3000.0f;
	Falloff = RIF_Linear;
	ForceStrength = -999999.0f;
	bAutoActivate = true;

	// by default we affect all 'dynamic' objects that can currently be affected by forces
	AddCollisionChannelToAffect(ECC_Pawn);
	AddCollisionChannelToAffect(ECC_PhysicsBody);
	AddCollisionChannelToAffect(ECC_Vehicle);
	AddCollisionChannelToAffect(ECC_Destructible);

	UpdateCollisionObjectQueryParams();
}


// Called when the game starts
void UWindSuckerComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateCollisionObjectQueryParams();
}


// Called every frame
void UWindSuckerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsActive())
	{
		const FVector Origin = GetComponentLocation();

		// Find objects within the sphere
		TArray<FOverlapResult> Overlaps;

		FCollisionQueryParams Params(SCENE_QUERY_STAT(AddForceOverlap), false);
		Params.AddIgnoredActor(GetOwner());

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

void UWindSuckerComponent::AddCollisionChannelToAffect(enum ECollisionChannel CollisionChannel)
{
	EObjectTypeQuery ObjectType = UEngineTypes::ConvertToObjectType(CollisionChannel);
	if (ObjectType != ObjectTypeQuery_MAX)
	{
		AddObjectTypeToAffect(ObjectType);
	}
}

void UWindSuckerComponent::AddObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery> ObjectType)
{
	ObjectTypesToAffect.AddUnique(ObjectType);
	UpdateCollisionObjectQueryParams();
}

void UWindSuckerComponent::RemoveObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery> ObjectType)
{
	ObjectTypesToAffect.Remove(ObjectType);
	UpdateCollisionObjectQueryParams();
}

void UWindSuckerComponent::UpdateCollisionObjectQueryParams()
{
	CollisionObjectQueryParams = FCollisionObjectQueryParams(ObjectTypesToAffect);
}