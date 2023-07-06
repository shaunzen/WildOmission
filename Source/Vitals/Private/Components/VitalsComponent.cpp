// Copyright Telephone Studios. All Rights Reserved.


#include "Components/VitalsComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

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

	BeginThirstBroadcasted = false;
	EndThirstBroadcasted = false;

	BeginStarvingBroadcasted = false;
	EndStarvingBroadcasted = false;

	ConstructorHelpers::FObjectFinder<USoundBase> DefaultHurtSoundObject(TEXT("/Game/WildOmissionCore/Characters/Human/Audio/Damage/Human_HurtGeneric_Cue"));
	if (!DefaultHurtSoundObject.Succeeded())
	{
		return;
	}

	HurtSound = DefaultHurtSoundObject.Object;
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
	HandleDelegates();

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	// Remove from thirst and hunger, and clamp to min and max value
	CurrentThirst = FMath::Clamp(CurrentThirst - (ThirstDepletionRate * GetWorld()->GetDeltaSeconds()), 0.0f, MaxThirst);
	CurrentHunger = FMath::Clamp(CurrentHunger - (HungerDepletionRate * GetWorld()->GetDeltaSeconds()), 0.0f, MaxHunger);
	
	// If Thirst or Hunger is below threshold start removing Health
	if (IsThirsty() || IsStarving())
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - (HealthDepletionRate * GetWorld()->GetDeltaSeconds()), 0.0f, MaxHealth);
	}

	// If Health is too low kill the player
	if (CurrentHealth < KINDA_SMALL_NUMBER)
	{
		CurrentHealth = 0.0f;
		OnHealthDepleted.Broadcast();
	}
}

void UVitalsComponent::HandleDelegates()
{
	if (!IsStarving())
	{
		BroadcastEndStarving();
	}
	else
	{
		BroadcastBeginStarving();
	}

	if (!IsThirsty())
	{
		BroadcastEndThirst();
	}
	else
	{
		BroadcastBeginThirst();
	}
}

void UVitalsComponent::BroadcastBeginThirst()
{
	if (BeginThirstBroadcasted || !OnBeginThirst.IsBound())
	{
		return;
	}

	OnBeginThirst.Broadcast(GetWorld()->GetRealTimeSeconds());
	BeginThirstBroadcasted = true;
	EndThirstBroadcasted = false;
}

void UVitalsComponent::BroadcastEndThirst()
{
	if (EndThirstBroadcasted || !OnEndThirst.IsBound())
	{
		return;
	}

	OnEndThirst.Broadcast(GetWorld()->GetRealTimeSeconds());
	BeginThirstBroadcasted = false;
	EndThirstBroadcasted = true;
}

void UVitalsComponent::BroadcastBeginStarving()
{
	if (BeginStarvingBroadcasted || !OnBeginStarving.IsBound())
	{
		return;
	}

	OnBeginStarving.Broadcast(GetWorld()->GetRealTimeSeconds());
	BeginStarvingBroadcasted = true;
	EndStarvingBroadcasted = false;

}

void UVitalsComponent::BroadcastEndStarving()
{
	if (EndStarvingBroadcasted || !OnEndStarving.IsBound())
	{
		return;
	}

	OnEndStarving.Broadcast(GetWorld()->GetRealTimeSeconds());
	BeginStarvingBroadcasted = false;
	EndStarvingBroadcasted = true;
}

void UVitalsComponent::SetHealth(float Value)
{
	CurrentHealth = FMath::Clamp(Value, 0.0f, MaxHealth);
}

void UVitalsComponent::SetThirst(float Value)
{
	CurrentThirst = FMath::Clamp(Value, 0.0f, MaxThirst);
}

void UVitalsComponent::SetHunger(float Value)
{
	CurrentHunger = FMath::Clamp(Value, 0.0f, MaxHunger);
}

void UVitalsComponent::AddHealth(float Value)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Value, 0.0f, MaxHealth);
	if (Value < 0.0f)
	{
		Client_PlayHurtSound();
	}
}

void UVitalsComponent::AddThirst(float Value)
{
	CurrentThirst = FMath::Clamp(CurrentThirst + Value, 0.0f, MaxThirst);
}

void UVitalsComponent::AddHunger(float Value)
{
	CurrentHunger = FMath::Clamp(CurrentHunger + Value, 0.0f, MaxHunger);
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

bool UVitalsComponent::IsThirsty() const
{
	return CurrentThirst < ThirstThreshold;
}

bool UVitalsComponent::IsStarving() const
{
	return CurrentHunger < HungerThreshold;
}

void UVitalsComponent::Client_PlayHurtSound_Implementation()
{
	if (HurtSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HurtSound, GetOwner()->GetActorLocation());
}

void UVitalsComponent::OnOwnerTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetOwner()->HasAuthority() || DamagedActor == nullptr)
	{
		return;
	}

	AddHealth(-Damage);
}