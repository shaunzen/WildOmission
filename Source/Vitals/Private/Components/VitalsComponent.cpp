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

	ThirstThreshold = MaxThirst * 0.25f;
	HungerThreshold = MaxHunger * 0.25f;

	HealthCanDeplete = true;
	ThirstCanDeplete = true;
	HungerCanDeplete = true;

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

	if (GetOwner() == nullptr)
	{
		return;
	}
	
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

	DOREPLIFETIME_CONDITION(UVitalsComponent, CurrentHealth, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UVitalsComponent, CurrentThirst, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UVitalsComponent, CurrentHunger, COND_OwnerOnly);
}

void UVitalsComponent::CalculateDepletion()
{
	HandleDelegates();

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	// Remove from thirst and hunger, and clamp to min and max value
	if (ThirstCanDeplete)
	{
		CurrentThirst = FMath::Clamp(CurrentThirst - (ThirstDepletionRate * GetWorld()->GetDeltaSeconds()), 0.0f, MaxThirst);
	}
	if (HungerCanDeplete)
	{
		CurrentHunger = FMath::Clamp(CurrentHunger - (HungerDepletionRate * GetWorld()->GetDeltaSeconds()), 0.0f, MaxHunger);
	}
	
	// If Thirst or Hunger is below threshold start removing Health
	if ((IsThirsty() || IsStarving()) && HealthCanDeplete)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - (HealthDepletionRate * GetWorld()->GetDeltaSeconds()), 0.0f, MaxHealth);
	}

	// If Health is too low kill the player
	if (CurrentHealth <= 0.0f)
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
	if (Value < 0.0f && !HealthCanDeplete)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + Value, 0.0f, MaxHealth);
	if (Value < 0.0f)
	{
		if (OnTakeAnyDamage.IsBound())
		{
			OnTakeAnyDamage.Broadcast();
		}
		Client_PlayHurtSound();
	}
}

void UVitalsComponent::AddThirst(float Value)
{
	if (Value < 0.0f && !ThirstCanDeplete)
	{
		return;
	}

	CurrentThirst = FMath::Clamp(CurrentThirst + Value, 0.0f, MaxThirst);
}

void UVitalsComponent::AddHunger(float Value)
{
	if (Value < 0.0f && !HungerCanDeplete)
	{
		return;
	}

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

void UVitalsComponent::SetHealthCanDeplete(bool InCanDeplete)
{
	HealthCanDeplete = InCanDeplete;
}

void UVitalsComponent::SetThirstCanDeplete(bool InCanDeplete)
{
	ThirstCanDeplete = InCanDeplete;
}

void UVitalsComponent::SetHungerCanDeplete(bool InCanDeplete)
{
	HungerCanDeplete = InCanDeplete;
}

bool UVitalsComponent::IsThirsty() const
{
	return CurrentThirst < ThirstThreshold;
}

bool UVitalsComponent::IsStarving() const
{
	return CurrentHunger < HungerThreshold;
}

void UVitalsComponent::SetGodMode(bool InGodMode)
{
	HealthCanDeplete = !InGodMode;
	ThirstCanDeplete = !InGodMode;
	HungerCanDeplete = !InGodMode;
}

bool UVitalsComponent::IsGodMode() const
{
	return !HealthCanDeplete && !ThirstCanDeplete && !HungerCanDeplete;
}

void UVitalsComponent::SetHurtSound(USoundBase* InHurtSound)
{
	HurtSound = InHurtSound;
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
	if (GetOwner() == nullptr || !GetOwner()->HasAuthority() || DamagedActor == nullptr)
	{
		return;
	}

	AddHealth(-Damage);
}