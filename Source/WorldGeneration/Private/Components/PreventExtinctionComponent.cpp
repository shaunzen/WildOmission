// Copyright Telephone Studios. All Rights Reserved.


#include "Components/PreventExtinctionComponent.h"
#include "WorldGenerationHandler.h"

// Sets default values for this component's properties
UPreventExtinctionComponent::UPreventExtinctionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPreventExtinctionComponent::BeginPlay()
{
	Super::BeginPlay();

	AWorldGenerationHandler* OwnerWorldGenerationHandler = Cast<AWorldGenerationHandler>(GetOwner());
	if (OwnerWorldGenerationHandler == nullptr)
	{
		return;
	}

	TArray<FBiomeGenerationData*> Biomes = OwnerWorldGenerationHandler->GetAllPossibleBiomes();
	if (Biomes.IsEmpty())
	{
		return;
	}


}
