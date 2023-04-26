// Copyright Telephone Studios. All Rights Reserved.


#include "BuildAnchorComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UBuildAnchorComponent::UBuildAnchorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Type = EBuildAnchorType::FoundationAnchor;
	AttachedDeployable = nullptr;
}

void UBuildAnchorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildAnchorComponent, AttachedDeployable);
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

ADeployable* UBuildAnchorComponent::GetAttachedDeployable() const
{
	return AttachedDeployable;
}

void UBuildAnchorComponent::AttachDeployable(ADeployable* DeployableToAttach)
{
	AttachedDeployable = DeployableToAttach;
}

TArray<UBuildAnchorComponent*> UBuildAnchorComponent::GetAllBuildAnchorsOfTypeFromList(const TArray<UBuildAnchorComponent*>& BuildAnchorList, TEnumAsByte<EBuildAnchorType> TypeToFind)
{
	TArray<UBuildAnchorComponent*> SortedBuildAnchorList;
	for (UBuildAnchorComponent* BuildAnchorComponent : BuildAnchorList)
	{
		if (BuildAnchorComponent->GetType() != TypeToFind)
		{
			continue;
		}
		SortedBuildAnchorList.Add(BuildAnchorComponent);
	}
	return SortedBuildAnchorList;
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

	return ClosestAnchor;
}