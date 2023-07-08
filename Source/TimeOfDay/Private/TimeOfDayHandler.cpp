// Copyright Telephone Studios. All Rights Reserved.


#include "TimeOfDayHandler.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Log.h"

static const float DAY_NIGHT_SPEED = 0.3f;
static UMaterialParameterCollection* MPC_Sky = nullptr;

// Sets default values
ATimeOfDayHandler::ATimeOfDayHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DirectionalLight = nullptr;
	NormalizedProgressThroughDay = 0.0f;
	DaysPlayed = 0;

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> SkyCollectionBlueprint(TEXT("/Game/WildOmissionCore/Art/Environment/MPC_Sky"));
	if (SkyCollectionBlueprint.Succeeded())
	{
		MPC_Sky = SkyCollectionBlueprint.Object;
	}
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

void ATimeOfDayHandler::CalculateMoonPhase()
{
	float MoonPhase = (static_cast<float>(DaysPlayed) / 30.0f) - FMath::Floor((static_cast<float>(DaysPlayed) / 30.0f));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Sky, TEXT("MoonPhase"), MoonPhase);
}

// Called every frame
void ATimeOfDayHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DirectionalLight == nullptr)
	{
		return;
	}

	// TODO setup as normal cycle time
	DirectionalLight->AddActorLocalRotation(FRotator(10.0f * DeltaTime, 0.0f, 0.0f));
	NormalizedProgressThroughDay += (10.0f * DeltaTime / 360.0f);
	
	if (NormalizedProgressThroughDay >= 1.0f)
	{
		DaysPlayed++;
		NormalizedProgressThroughDay = 0.0f;
		CalculateMoonPhase();
	}
}

void ATimeOfDayHandler::SetDaysPlayed(int32 InDaysPlayed)
{
	DaysPlayed = InDaysPlayed;
	CalculateMoonPhase();
}

int32 ATimeOfDayHandler::GetDaysPlayed() const
{
	return DaysPlayed;
}

void ATimeOfDayHandler::SetNormalizedProgressThroughDay(float InProgress)
{
	DirectionalLight->SetActorRotation(FRotator(0.0f, 180.0f, 180.0f));
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