// Copyright Telephone Studios. All Rights Reserved.


#include "WeatherHandlerComponent.h"
#include "Storm.h"
#include "UObject/ConstructorHelpers.h"
#include "WildOmission/Core/WildOmissionStatics.h"

// Sets default values for this component's properties
UWeatherHandlerComponent::UWeatherHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AStorm> StormBlueprint(TEXT("/Game/WildOmission/Weather/BP_Storm"));
	if (StormBlueprint.Succeeded())
	{
		StormClass = StormBlueprint.Class;
	}

	MinTimeBetweenStorms = 300.0f;
	MaxTimeBetweenStorms = 3600.0f;
	NextStormChanceTime = 0.0f;
}


// Called when the game starts
void UWeatherHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (NextStormChanceTime == 0.0f)
	{
		GetNewStormChanceTime();
	}
}


// Called every frame
void UWeatherHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	NextStormChanceTime -= DeltaTime;
	if (NextStormChanceTime < KINDA_SMALL_NUMBER)
	{
		TrySpawnStorm();
		GetNewStormChanceTime();
	}
}

float UWeatherHandlerComponent::GetNextStormChanceTime() const
{
	return NextStormChanceTime;
}

void UWeatherHandlerComponent::SetNextStormChanceTime(float NewTime)
{
	NextStormChanceTime = NewTime;
}

void UWeatherHandlerComponent::GetNewStormChanceTime()
{
	NextStormChanceTime = FMath::RandRange(MinTimeBetweenStorms, MaxTimeBetweenStorms);
}

void UWeatherHandlerComponent::TrySpawnStorm()
{
	bool WillSpawn = FMath::RandBool();

	if (WillSpawn == false)
	{
		return;
	}

	FVector2D WorldSize;

	UWildOmissionStatics::GetWorldSize(GetWorld(), WorldSize);

	AStorm* SpawnedStorm = GetWorld()->SpawnActor<AStorm>(StormClass);
	SpawnedStorm->OnSpawn(WorldSize);
}