// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/BuildingPart.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABuildingPart::ABuildingPart()
{
	Stability = 100.0f;
}

void ABuildingPart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuildingPart, Stability);
}

float ABuildingPart::GetStability() const
{
	return Stability;
}

void ABuildingPart::CalculateStability(float OwnerStability)
{
	if (CanSpawnOnAnchor == EBuildAnchorType::FoundationAnchor)
	{
		Stability = 100.0f;
		return;
	}

	Stability = OwnerStability * 0.9f;

	if (HasAuthority() && Stability < KINDA_SMALL_NUMBER)
	{
		Destroy();
	}

	CalculateTouchingBuildingPartsStability();
}

void ABuildingPart::CalculateTouchingBuildingPartsStability()
{
	TArray<UActorComponent*> ChildBuildAnchorsActorComponents;
	ChildBuildAnchorsActorComponents = GetComponentsByClass(UBuildAnchorComponent::StaticClass());
	for (UActorComponent* ChildActorComp : ChildBuildAnchorsActorComponents)
	{
		UBuildAnchorComponent* ChildBuildAnchor = Cast<UBuildAnchorComponent>(ChildActorComp);
		if (ChildBuildAnchor == nullptr)
		{
			continue;
		}

		TArray<AActor*> OverlappingActors;
		ChildBuildAnchor->GetOverlappingActors(OverlappingActors, ABuildingPart::StaticClass());
		for (AActor* OverlappingActor : OverlappingActors)
		{
			ABuildingPart* OverlappingBuildingPart = Cast<ABuildingPart>(OverlappingActor);
			if (OverlappingBuildingPart == nullptr)
			{
				continue;
			}
			OverlappingBuildingPart->CalculateStability(GetStability());
		}
	}
}
