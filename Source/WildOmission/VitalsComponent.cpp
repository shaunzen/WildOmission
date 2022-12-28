// (c) 2023 Telephone Studios, all rights reserved.


#include "VitalsComponent.h"

// Sets default values for this component's properties
UVitalsComponent::UVitalsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	MaxThirst = 300.0f;
	MaxHunger = 500.0f;

	HealthDepletionRate = 1.0f;
	ThirstDepletionRate = 0.1f;
	HungerDepletionRate = 0.3f;

	ThirstThreshold = 30.0f;
	HungerThreshold = 30.0f;

	CurrentHealth = 100.0f;
	CurrentThirst = 300.0f;
	CurrentHunger = 500.0f;
}


// Called when the game starts
void UVitalsComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UVitalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Run Thirst and Hunger depletion calculations
	CurrentThirst -= ThirstDepletionRate * DeltaTime;
	CurrentHunger -= HungerDepletionRate * DeltaTime;

	// Prevent from being less than 0
	if (CurrentThirst < 0.0f)
	{
		CurrentThirst = 0.0f;
	}
	if (CurrentHunger < 0.0f)
	{
		CurrentHunger = 0.0f;
	}

	// If Thirst or Hunger is below threshold start removing Health
	if (CurrentThirst < ThirstThreshold || CurrentHunger < HungerThreshold)
	{
		CurrentHealth -= HealthDepletionRate * DeltaTime;
	}

	// If Health is less than 0 kill the player
	if (CurrentHealth < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Has Died"));
		CurrentHealth = 0.0f;
	}
}

float UVitalsComponent::GetHealth()
{
	return CurrentHealth;
}

float UVitalsComponent::GetThirst()
{
	return CurrentThirst;
}

float UVitalsComponent::GetHunger()
{
	return CurrentHunger;
}

void UVitalsComponent::SetHealth(float Value)
{
	CurrentHealth = Value;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	else if (CurrentHealth < 0.0f)
	{
		CurrentHealth = 0.0f;
	}
}

void UVitalsComponent::SetThirst(float Value)
{
	CurrentThirst = Value;
	if (CurrentThirst > MaxThirst)
	{
		CurrentThirst = MaxThirst;
	}
	else if (CurrentThirst < 0.0f)
	{
		CurrentThirst = 0.0f;
	}
}

void UVitalsComponent::SetHunger(float Value)
{
	CurrentHunger = Value;
	if (CurrentHunger > MaxHunger)
	{
		CurrentHunger = MaxHunger;
	}
	else if (CurrentHunger < 0.0f)
	{
		CurrentHunger = 0.0f;
	}
}

void UVitalsComponent::LogVitals()
{
	UE_LOG(LogTemp, Display, TEXT("Health: %f, Thirst: %f, Hunger: %f"), CurrentHealth, CurrentThirst, CurrentHunger);
}