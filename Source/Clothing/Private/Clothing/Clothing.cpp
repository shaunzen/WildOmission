// Copyright Telephone Studios. All Rights Reserved.


#include "Clothing/Clothing.h"

// Sets default values for this component's properties
UClothing::UClothing()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UClothing::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UClothing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TEnumAsByte<EClothingType> UClothing::GetType() const
{
	return Type;
}

