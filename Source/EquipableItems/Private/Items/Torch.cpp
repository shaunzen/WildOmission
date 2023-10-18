// Copyright Telephone Studios. All Rights Reserved.


#include "Items/Torch.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"

ATorch::ATorch()
{
	ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetAutoActivate(false);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetAutoActivate(false);
	
	IsBurning = false;
}
