// Copyright Telephone Studios. All Rights Reserved.


#include "WeatherManager.h"
#include "Storm.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Core/WildOmissionStatics.h"

// Sets default values
AWeatherManager::AWeatherManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FClassFinder<AStorm> StormBlueprint(TEXT("/Game/WildOmission/Weather/BP_Storm"));
	if (StormBlueprint.Succeeded())
	{
		StormClass = StormBlueprint.Class;
	}

	MinTimeBetweenStorms = 300.0f;
	MaxTimeBetweenStorms = 3600.0f;
	NextStormChanceTime = 0.0f;
}

// Called when the game starts or when spawned
void AWeatherManager::BeginPlay()
{
	Super::BeginPlay();

	if (NextStormChanceTime == 0.0f)
	{
		GetNewStormChanceTime();
	}
}

// Called every frame
void AWeatherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NextStormChanceTime -= DeltaTime;
	if (NextStormChanceTime < KINDA_SMALL_NUMBER)
	{
		TrySpawnStorm();
		GetNewStormChanceTime();
	}
}

float AWeatherManager::GetNextStormChanceTime() const
{
	return NextStormChanceTime;
}

void AWeatherManager::SetNextStormChanceTime(float NewTime)
{
	NextStormChanceTime = NewTime;
}

void AWeatherManager::GetNewStormChanceTime()
{
	NextStormChanceTime = FMath::RandRange(MinTimeBetweenStorms, MaxTimeBetweenStorms);
}

void AWeatherManager::TrySpawnStorm()
{
	bool WillSpawn = FMath::RandBool();

	if (WillSpawn == false)
	{
		return;
	}

	SpawnStorm();
}

AStorm* AWeatherManager::SpawnStorm()
{
	FVector2D WorldSize;

	UWildOmissionStatics::GetWorldSize(GetWorld(), WorldSize);

	AStorm* SpawnedStorm = GetWorld()->SpawnActor<AStorm>(StormClass);
	SpawnedStorm->OnSpawn(WorldSize);

	return SpawnedStorm;
}

void AWeatherManager::ClearAllStorms()
{
	TArray<AActor*> StormActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorm::StaticClass(), StormActors);
	
	for (AActor* StormActor : StormActors)
	{
		AStorm* Storm = Cast<AStorm>(StormActor);
		if (Storm == nullptr)
		{
			continue;
		}

		Storm->HandleDestruction();
	}
}