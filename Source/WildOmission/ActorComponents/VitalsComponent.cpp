// Copyright Telephone Studios. All Rights Reserved.


#include "VitalsComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UVitalsComponent::UVitalsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	MaxThirst = 300.0f;
	MaxHunger = 500.0f;

	/*
	HealthDepletionRate = 1.0f;
	ThirstDepletionRate = 0.1f;
	HungerDepletionRate = 0.3f;
	*/

	HealthDepletionRate = 10.0f;
	ThirstDepletionRate = 10.0f;
	HungerDepletionRate = 10.0f;

	ThirstThreshold = 30.0f;
	HungerThreshold = 30.0f;

	ReplicatedCurrentHealth = 100.0f;
	ReplicatedCurrentThirst = 300.0f;
	ReplicatedCurrentHunger = 500.0f;

	CurrentHealth = 100.0f;
	CurrentThirst = 300.0f;
	CurrentHunger = 500.0f;
}


// Called when the game starts
void UVitalsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		SetIsReplicated(true);
	}
}


// Called every frame
void UVitalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateDepletion();
}

void UVitalsComponent::CalculateDepletion()
{
	if (GetOwner()->HasAuthority())
	{

		// Run Thirst and Hunger depletion calculations
		ReplicatedCurrentThirst -= ThirstDepletionRate * GetWorld()->GetDeltaSeconds();
		ReplicatedCurrentHunger -= HungerDepletionRate * GetWorld()->GetDeltaSeconds();


		// Prevent from being less than 0
		FMath::Clamp(ReplicatedCurrentThirst, 0, MaxThirst);
		FMath::Clamp(ReplicatedCurrentHunger, 0, MaxHunger);

		// If Thirst or Hunger is below threshold start removing Health
		if (ReplicatedCurrentThirst < ThirstThreshold || ReplicatedCurrentHunger < HungerThreshold)
		{
			ReplicatedCurrentHealth -= HealthDepletionRate * GetWorld()->GetDeltaSeconds();
		}

		// If Health is less than 0 kill the player
		if (ReplicatedCurrentHealth < 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Has Died"));
			ReplicatedCurrentHealth = 0.0f;
		}
		CurrentHealth = ReplicatedCurrentHealth;
		CurrentThirst = ReplicatedCurrentThirst;
		CurrentHunger = ReplicatedCurrentHunger;
	}
	else
	{
		CurrentHealth = ReplicatedCurrentHealth;
		CurrentThirst = ReplicatedCurrentThirst;
		CurrentHunger = ReplicatedCurrentHunger;
	}
}

float UVitalsComponent::GetMaxHealth()
{
	return MaxHealth;
}

float UVitalsComponent::GetMaxThirst()
{
	return MaxThirst;
}

float UVitalsComponent::GetMaxHunger()
{
	return MaxHunger;
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
	if (GetOwner()->HasAuthority())
	{
		ReplicatedCurrentHealth = Value;
		if (ReplicatedCurrentHealth > MaxHealth)
		{
			ReplicatedCurrentHealth = MaxHealth;
		}
		else if (ReplicatedCurrentHealth < 0.0f)
		{
			ReplicatedCurrentHealth = 0.0f;
		}
	}
}

void UVitalsComponent::SetThirst(float Value)
{
	if (GetOwner()->HasAuthority())
	{
		ReplicatedCurrentThirst = Value;
		if (ReplicatedCurrentThirst > MaxThirst)
		{
			ReplicatedCurrentThirst = MaxThirst;
		}
		else if (ReplicatedCurrentThirst < 0.0f)
		{
			ReplicatedCurrentThirst = 0.0f;
		}
	}
	else
	{
		CurrentHealth = ReplicatedCurrentHealth;
		CurrentThirst = ReplicatedCurrentThirst;
		CurrentHunger = ReplicatedCurrentHunger;
	}
}

void UVitalsComponent::SetHunger(float Value)
{
	if (GetOwner()->HasAuthority())
	{
		ReplicatedCurrentHunger = Value;
		if (ReplicatedCurrentHunger > MaxHunger)
		{
			ReplicatedCurrentHunger = MaxHunger;
		}
		else if (ReplicatedCurrentHunger < 0.0f)
		{
			ReplicatedCurrentHunger = 0.0f;
		}
	}
	else
	{
		CurrentHealth = ReplicatedCurrentHealth;
		CurrentThirst = ReplicatedCurrentThirst;
		CurrentHunger = ReplicatedCurrentHunger;
	}
}

void UVitalsComponent::LogVitals()
{
	UE_LOG(LogTemp, Display, TEXT("Health: %f, Thirst: %f, Hunger: %f"), CurrentHealth, CurrentThirst, CurrentHunger);
}

void UVitalsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVitalsComponent, ReplicatedCurrentHealth);
	DOREPLIFETIME(UVitalsComponent, ReplicatedCurrentThirst);
	DOREPLIFETIME(UVitalsComponent, ReplicatedCurrentHunger);
}