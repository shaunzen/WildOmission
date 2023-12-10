// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/AirRaidSiren.h"
#include "WeatherManager.h"
#include "Actors/Storm.h"
#include "Actors/Tornado.h"
#include "Components/AudioComponent.h"

AAirRaidSiren::AAirRaidSiren()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetAutoActivate(false);
	AudioComponent->SetupAttachment(RootComponent);
}

void AAirRaidSiren::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FTimerHandle CheckForTornadoTimerHandle;
	FTimerDelegate CheckForTornadoTimerDelegate;
	CheckForTornadoTimerDelegate.BindUObject(this, &AAirRaidSiren::CheckForTornado);

	World->GetTimerManager().SetTimer(CheckForTornadoTimerHandle, CheckForTornadoTimerDelegate, 10.0f, true);
}

void AAirRaidSiren::SoundAlarm()
{
	UWorld* World = GetWorld();
	if (World == nullptr || AudioComponent == nullptr)
	{
		return;
	}

	if (AudioComponent)
	{
		AudioComponent->Activate();
		AudioComponent->Play();
	}

	if (MeshComponent)
	{
		MeshComponent->SetCustomPrimitiveDataFloat(0, 1.0f);
	}

	FTimerHandle StopAlarmTimerHandle;
	World->GetTimerManager().SetTimer(StopAlarmTimerHandle, 60.0f, false);
	
	CurrentlyActive = true;
}

void AAirRaidSiren::StopAlarm()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->Deactivate();
	}

	if (MeshComponent)
	{
		MeshComponent->SetCustomPrimitiveDataFloat(0, 0.0f);
	}

	CurrentlyActive = false;
}

void AAirRaidSiren::CheckForTornado()
{
	if (CurrentlyActive)
	{
		return;
	}
	
	// Get The Weather Manager
	AWeatherManager* WeatherManager = AWeatherManager::GetWeatherManager();
	if (WeatherManager == nullptr)
	{
		return;
	}

	// Check If Storm
	AStorm* SpawnedStorm = WeatherManager->GetCurrentStorm();
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