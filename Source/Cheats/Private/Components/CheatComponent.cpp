// Copyright Telephone Studios. All Rights Reserved.


#include "Components/CheatComponent.h"
#include "UI/CheatMenuWidget.h"

// Sets default values for this component's properties
UCheatComponent::UCheatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CheatMenuWidget = nullptr;
}

void UCheatComponent::ToggleMenu()
{
	if (CheatMenuWidget != nullptr)
	{
		CheatMenuWidget->Teardown();
		CheatMenuWidget = nullptr;
		return;
	}

}

// Called when the game starts
void UCheatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCheatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

