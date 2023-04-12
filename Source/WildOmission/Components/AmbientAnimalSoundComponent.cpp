// Copyright Telephone Studios. All Rights Reserved.


#include "AmbientAnimalSoundComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAmbientAnimalSoundComponent::UAmbientAnimalSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UAmbientAnimalSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	float Rate = FMath::RandRange(0.3f, 5.0f);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAmbientAnimalSoundComponent::PlaySoundCue, Rate, true);
}

void UAmbientAnimalSoundComponent::PlaySoundCue()
{
	if (AnimalCue == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AnimalCue, GetComponentLocation());
}