// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/TornadoSiren.h"
#include "WeatherHandler.h"
#include "Actors/Storm.h"
#include "Actors/Tornado.h"
#include "Components/AudioComponent.h"

ATornadoSiren::ATornadoSiren()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetAutoActivate(false);
	AudioComponent->SetupAttachment(RootComponent);
}

void ATornadoSiren::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FTimerHandle CheckForTornadoTimerHandle;
	FTimerDelegate CheckForTornadoTimerDelegate;
	CheckForTornadoTimerDelegate.BindUObject(this, &ATornadoSiren::CheckForTornado);

	World->GetTimerManager().SetTimer(CheckForTornadoTimerHandle, CheckForTornadoTimerDelegate, 10.0f, true);
}

void ATornadoSiren::SoundAlarm()
{
	UWorld* World = GetWorld();
	if (World == nullptr || AudioComponent == nullptr)
	{
		return;
	}

	AudioComponent->Activate();
	AudioComponent->Play();

	FTimerHandle StopAlarmTimerHandle;
	World->GetTimerManager().SetTimer(StopAlarmTimerHandle, 15.0f, false);
}

void ATornadoSiren::StopAlarm()
{
	if (AudioComponent == nullptr)
	{
		return;
	}

	AudioComponent->Stop();
	AudioComponent->Deactivate();
}

void ATornadoSiren::CheckForTornado()
{
	// Get The Weather Handler
	AWeatherHandler* WeatherHandler = AWeatherHandler::GetWeatherHandler();
	if (WeatherHandler == nullptr)
	{
		return;
	}

	// Check If Storm
	AStorm* SpawnedStorm = WeatherHandler->GetCurrentStorm();
	if (SpawnedStorm == nullptr)
	{
		return;
	}

	// If Storm Is More Than 90% Severe Or Has Tornado
	if (SpawnedStorm->GetSeverity() < 90.0f || SpawnedStorm->GetSpawnedTornado() == nullptr)
	{
		return;
	}

	// Sound Alarm
	SoundAlarm();
}