// Copyright Telephone Studios. All Rights Reserved.


#include "AmbientSoundProducerComponent.h"
#include "TimeOfDayHandler.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UAmbientSoundProducerComponent::UAmbientSoundProducerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	DayCue = nullptr;
	NightCue = nullptr;
	
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
}


// Called when the game starts
void UAmbientSoundProducerComponent::BeginPlay()
{
	Super::BeginPlay();

	
	FTimerHandle TimerHandle;
	float Rate = FMath::RandRange(1.0f, 5.0f);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAmbientSoundProducerComponent::PlaySoundCue, Rate, true);
}

void UAmbientSoundProducerComponent::PlaySoundCue()
{
	if (DayCue == nullptr || NightCue == nullptr)
	{
		return;
	}

	ATimeOfDayHandler* TimeOfDayHandler = ATimeOfDayHandler::GetTimeOfDayHandler();
	if (TimeOfDayHandler && TimeOfDayHandler->IsNight())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), NightCue, GetComponentLocation());
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DayCue, GetComponentLocation());
	}
}