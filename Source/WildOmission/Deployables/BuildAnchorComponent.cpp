// Copyright Telephone Studios. All Rights Reserved.


#include "BuildAnchorComponent.h"

// Sets default values for this component's properties
UBuildAnchorComponent::UBuildAnchorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Type = EBuildAnchorType::FoundationAnchor;
	Occupied = false;
}


// Called when the game starts
void UBuildAnchorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

TEnumAsByte<EBuildAnchorType> UBuildAnchorComponent::GetType() const
{
	return Type;
}

bool UBuildAnchorComponent::IsOccupied() const
{
	return Occupied;
}

void UBuildAnchorComponent::SetOccupied(bool bOccupied)
{
	Occupied = bOccupied;
}

TArray<UBuildAnchorComponent*> UBuildAnchorComponent::GetAllBuildAnchorsOfTypeFromList(const TArray<UActorComponent*>& ActorComponentList, TEnumAsByte<EBuildAnchorType> TypeToFind)
{
	TArray<UBuildAnchorComponent*> BuildAnchorList;
	for (UActorComponent* ActorComp : ActorComponentList)
	{
		UBuildAnchorComponent* BuildAnchorComponent = Cast<UBuildAnchorComponent>(ActorComp);
		if (BuildAnchorComponent == nullptr || BuildAnchorComponent->GetType() != TypeToFind)
		{
			continue;
		}
		BuildAnchorList.Add(BuildAnchorComponent);
	}
	return BuildAnchorList;
}

UBuildAnchorComponent* UBuildAnchorComponent::GetClosestBuildAnchorFromList(const TArray<UBuildAnchorComponent*>& BuildAnchorList, const FVector& TestPoint)
{
	int32 ShortestDistance = -1;
	UBuildAnchorComponent* ClosestAnchor = nullptr;
	for (UBuildAnchorComponent* BuildAnchor : BuildAnchorList)
	{
		FVector Difference = BuildAnchor->GetComponentLocation() - TestPoint;
		float Distance = Difference.Length();
		if (ShortestDistance == -1 || Distance < ShortestDistance)
		{
			ShortestDistance = Distance;
			ClosestAnchor = BuildAnchor;
		}
	}

	if (ClosestAnchor != nullptr && ClosestAnchor->IsOccupied())
	{
		return nullptr;
	}

	return ClosestAnchor;
}