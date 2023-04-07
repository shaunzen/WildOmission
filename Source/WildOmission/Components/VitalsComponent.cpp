// Copyright Telephone Studios. All Rights Reserved.


#include "VitalsComponent.h"
#include "Net/UnrealNetwork.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"

// Sets default values for this component's properties
UVitalsComponent::UVitalsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

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

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UVitalsComponent::OnOwnerTakeAnyDamage);
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

	// If Health is too low kill the player
	if (CurrentHealth < KINDA_SMALL_NUMBER)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player Has Died"));
		CurrentHealth = 0.0f;

		AWildOmissionCharacter* CharacterOwner = Cast<AWildOmissionCharacter>(GetOwner());
		if(CharacterOwner == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to kill player cannot cast vitals component owner to wo character"));
			return;
		}
		CharacterOwner->HandleDeath();
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

void UVitalsComponent::OnOwnerTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetOwner()->HasAuthority() || DamagedActor == nullptr)
	{
		return;
	}

	AddHealth(-Damage);
}