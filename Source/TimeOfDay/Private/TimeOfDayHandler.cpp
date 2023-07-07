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
	DaysPlayed = 0;
	Hour = 0;
	Minute = 0;
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

	DirectionalLight->AddActorLocalRotation(FRotator(DAY_NIGHT_SPEED * DeltaTime, 0.0f, 0.0f));

	//UE_LOG(LogTimeOfDay, Display, TEXT("Light Rotation: %s, IsDay: %i, IsNight: %i"), *DirectionalLight->GetActorRotation().ToCompactString(), IsDay(), IsNight());
}

bool ATimeOfDayHandler::IsDay() const
{
	return DirectionalLight->GetActorRotation().Pitch < 0.0f;
}

bool ATimeOfDayHandler::IsNight() const
{
	return !IsDay();
}

