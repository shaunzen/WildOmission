// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/TornadoSiren.h"
#include "Components/AudioComponent.h"

ATornadoSiren::ATornadoSiren()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
}
