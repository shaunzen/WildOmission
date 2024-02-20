// Copyright Telephone Studios. All Rights Reserved.


#include "AmbientSoundProducerComponent.h"
#include "TimeOfDayManager.h"
#include "WeatherManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UAmbientSoundProducerComponent::UAmbientSoundProducerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	DayCue = nullptr;
	NightCue = nullptr;
	WindCue = nullptr;

	static ConstructorHelpers::FObjectFinder<USoundBase> DaySoundObject(TEXT("/Game/GatherableResources/Sounds/Birds/BirdCall_Cue"));
	if (DaySoundObject.Succeeded())
	{
		DayCue = DaySoundObject.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase> NightSoundObject(TEXT("/Game/GatherableResources/Sounds/Crickets/Crickets_Cue"));
	if (NightSoundObject.Succeeded())
	{
		NightCue = NightSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> WindSoundObject(TEXT("/Game/GatherableResources/Sounds/Wind/WindHiss_Cue"));
	if (WindSoundObject.Succeeded())
	{
		WindCue = WindSoundObject.Object;
	}
}


// Called when the game starts
void UAmbientSoundProducerComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FTimerHandle TimerHandle;
	const float Rate = FMath::RandRange(1.0f, 5.0f);

	World->GetTimerManager().SetTimer(TimerHandle, this, &UAmbientSoundProducerComponent::PlaySoundCue, Rate, true);
}

void UAmbientSoundProducerComponent::PlaySoundCue()
{
	ATimeOfDayManager* TimeOfDayManager = ATimeOfDayManager::GetTimeOfDayManager();
	if (TimeOfDayManager == nullptr)
	{
		return;
	}
	
	AWeatherManager* WeatherManager = AWeatherManager::GetWeatherManager();
	if (WeatherManager && WeatherManager->GetCurrentStorm() && UKismetMathLibrary::RandomBoolWithWeight(0.1f))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WindCue, this->GetComponentLocation());
	}

	TimeOfDayManager->IsNight() ? UGameplayStatics::PlaySoundAtLocation(GetWorld(), NightCue, this->GetComponentLocation()) :
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DayCue, this->GetComponentLocation());
}