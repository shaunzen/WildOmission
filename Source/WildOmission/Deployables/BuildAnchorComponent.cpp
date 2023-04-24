// Copyright Telephone Studios. All Rights Reserved.


#include "BuildAnchorComponent.h"

// Sets default values for this component's properties
UBuildAnchorComponent::UBuildAnchorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Type = EBuildAnchorType::FoundationAnchor;
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