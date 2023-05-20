// Copyright Telephone Studios. All Rights Reserved.


#include "WeatherHandlerComponent.h"

// Sets default values for this component's properties
UWeatherHandlerComponent::UWeatherHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeatherHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UWeatherHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

