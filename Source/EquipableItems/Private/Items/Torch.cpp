// Copyright Telephone Studios. All Rights Reserved.


#include "Items/Torch.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

ATorch::ATorch()
{
	PrimaryActorTick.bCanEverTick = false;

	ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(RootComponent);
	ParticleComponent->SetAutoActivate(false);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetAutoActivate(false);
	
	IsBurning = false;
}

void ATorch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATorch, IsBurning);
}

void ATorch::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();

	if (!HasAuthority())
	{
		return;
	}

	IsBurning = !IsBurning;
	OnRep_IsBurning();
}

void ATorch::OnRep_IsBurning()
{
	IsBurning ? StartFireEffects() : StopFireEffects();
}

void ATorch::DecrementDurability()
{
}

void ATorch::StartFireEffects()
{
	ParticleComponent->Activate();
	AudioComponent->Activate();

	if (HasAuthority())
	{
		FTimerDelegate DecrementDurabilityTimerDelegate;
	}
}

void ATorch::StopFireEffects()
{
	ParticleComponent->Deactivate();
	AudioComponent->Deactivate();
}
