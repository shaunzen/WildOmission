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

	StartHealth = MaxHealth * 0.9f;
	StartThirst = MaxThirst * 0.8f;
	StartHunger = MaxHunger * 0.8f;

	CurrentHealth = StartHealth;
	CurrentThirst = StartThirst;
	CurrentHunger = StartHunger;

	HealthDepletionRate = 1.0f;
	ThirstDepletionRate = 0.1f;
	HungerDepletionRate = 0.3f;

	ThirstThreshold = 30.0f;
	HungerThreshold = 30.0f;
}

// Called when the game starts
void UVitalsComponent::BeginPlay()
{
	Super::BeginPlay();

	SetIsReplicated(true);
	
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	// Set the start values
	SetHealth(StartHealth);
	SetThirst(StartThirst);
	SetHunger(StartHunger);
}

// Called every frame
void UVitalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateDepletion();
}

void UVitalsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVitalsComponent, CurrentHealth);
	DOREPLIFETIME(UVitalsComponent, CurrentThirst);
	DOREPLIFETIME(UVitalsComponent, CurrentHunger);
}

void UVitalsComponent::CalculateDepletion()
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	// Remove from thirst and hunger
	CurrentThirst -= ThirstDepletionRate * GetWorld()->GetDeltaSeconds();
	CurrentHunger -= HungerDepletionRate * GetWorld()->GetDeltaSeconds();

	// Prevent from being less than 0
	FMath::Clamp(CurrentThirst, 0, MaxThirst);
	FMath::Clamp(CurrentHunger, 0, MaxHunger);

	// If Thirst or Hunger is below threshold start removing Health
	if (CurrentThirst < ThirstThreshold || CurrentHunger < HungerThreshold)
	{
		CurrentHealth -= HealthDepletionRate * GetWorld()->GetDeltaSeconds();
	}

	// If Health is less than 0 kill the player
	if (CurrentHealth < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Has Died"));
		CurrentHealth = 0.0f;
	}
}

void UVitalsComponent::SetHealth(float Value)
{
	CurrentHealth = Value;
	FMath::Clamp(CurrentHealth, 0, MaxHealth);
}

void UVitalsComponent::SetThirst(float Value)
{
	CurrentThirst = Value;
	FMath::Clamp(CurrentThirst, 0, MaxThirst);
}

void UVitalsComponent::SetHunger(float Value)
{
	CurrentHunger = Value;
	FMath::Clamp(CurrentHunger, 0, MaxHunger);
}

void UVitalsComponent::AddHealth(float Value)
{
	CurrentHealth += Value;
	FMath::Clamp(CurrentHealth, 0, MaxHealth);
}

void UVitalsComponent::AddThirst(float Value)
{
	CurrentThirst += Value;
	FMath::Clamp(CurrentThirst, 0, MaxThirst);
}

void UVitalsComponent::AddHunger(float Value)
{
	CurrentHunger += Value;
	FMath::Clamp(CurrentHunger, 0, MaxHunger);
}

float UVitalsComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UVitalsComponent::GetMaxThirst() const
{
	return MaxThirst;
}

float UVitalsComponent::GetMaxHunger() const
{
	return MaxHunger;
}

float UVitalsComponent::GetHealth() const
{
	return CurrentHealth;
}

float UVitalsComponent::GetThirst() const
{
	return CurrentThirst;
}

float UVitalsComponent::GetHunger() const
{
	return CurrentHunger;
}