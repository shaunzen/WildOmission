// Copyright Telephone Studios. All Rights Reserved.


#include "TimeOfDayHandler.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

static const float DAY_NIGHT_SPEED = 0.3f;

// Sets default values
ATimeOfDayHandler::ATimeOfDayHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DirectionalLight = nullptr;
	NormalizedProgressThroughDay = 0.0f;
	DaysPlayed = 0;
}

// Called when the game starts or when spawned
void ATimeOfDayHandler::BeginPlay()
{
	Super::BeginPlay();
	
	DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight == nullptr)
	{
		UE_LOG(LogTimeOfDay, Error, TEXT("NO DIRECTIONAL LIGHT FOUND IN SCENE!"));
		return;
	}
}

// Called every frame
void ATimeOfDayHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DirectionalLight == nullptr)
	{
		return;
	}

	DirectionalLight->AddActorLocalRotation(FRotator(10.0f * DeltaTime, 0.0f, 0.0f));
	NormalizedProgressThroughDay += (10.0f * DeltaTime / 360.0f);
	
	if (NormalizedProgressThroughDay >= 1.0f)
	{
		DaysPlayed++;
		NormalizedProgressThroughDay = 0.0f;
	}

	UE_LOG(LogTimeOfDay, Display, TEXT("Progress through day: %f, Days Played: %i"), NormalizedProgressThroughDay, DaysPlayed);
	//UE_LOG(LogTimeOfDay, Display, TEXT("Light Rotation: %s, IsDay: %i, IsNight: %i"), *DirectionalLight->GetActorRotation().ToCompactString(), IsDay(), IsNight());
}

void ATimeOfDayHandler::SetDaysPlayed(int32 InDaysPlayed)
{
	DaysPlayed = InDaysPlayed;
}

int32 ATimeOfDayHandler::GetDaysPlayed() const
{
	return DaysPlayed;
}

void ATimeOfDayHandler::SetNormalizedProgressThroughDay(float InProgress)
{
	NormalizedProgressThroughDay = InProgress;
	DirectionalLight->AddActorLocalRotation(FRotator(NormalizedProgressThroughDay * 360.0f, 0.0f, 0.0f));
}

float ATimeOfDayHandler::GetNormalizedProgressThroughDay() const
{
	return NormalizedProgressThroughDay;
}

bool ATimeOfDayHandler::IsDay() const
{
	return DirectionalLight->GetActorRotation().Pitch < 0.0f;
}

bool ATimeOfDayHandler::IsNight() const
{
	return !IsDay();
}

