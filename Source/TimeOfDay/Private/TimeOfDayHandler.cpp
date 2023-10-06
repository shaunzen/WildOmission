// Copyright Telephone Studios. All Rights Reserved.


#include "TimeOfDayHandler.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Net/UnrealNetwork.h"
#include "Log.h"

static ATimeOfDayHandler* Instance = nullptr;
static const float DAY_NIGHT_SPEED = 0.3f;
static UMaterialParameterCollection* MPC_Sky = nullptr;

// Sets default values
ATimeOfDayHandler::ATimeOfDayHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 1.0f;

	DirectionalLight = nullptr;
	NormalizedProgressThroughDay = 0.0f;
	DaysPlayed = 0;

	SunriseBroadcasted = false;
	NoonBroadcasted = false;
	SunsetBroadcasted = false;
	MidnightBroadcasted = false;

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> SkyCollectionBlueprint(TEXT("/Game/WildOmissionCore/Art/Environment/MPC_Sky"));
	if (SkyCollectionBlueprint.Succeeded())
	{
		MPC_Sky = SkyCollectionBlueprint.Object;
	}
}

ATimeOfDayHandler* ATimeOfDayHandler::GetTimeOfDayHandler()
{
	return Instance;
}

// Called when the game starts or when spawned
void ATimeOfDayHandler::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (World == nullptr || World->IsEditorWorld() && IsValid(Instance))
	{
		return;
	}

	Instance = this;
	DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
}

void ATimeOfDayHandler::CalculateMoonPhase()
{
	if (MPC_Sky == nullptr)
	{
		UE_LOG(LogTimeOfDay, Error, TEXT("Couldn't calculate moon phase, MPC missing."));
		return;
	}
	
	const float MoonPhase = (static_cast<float>(DaysPlayed) / 8.0f) - FMath::Floor((static_cast<float>(DaysPlayed) / 8.0f));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Sky, TEXT("MoonPhase"), MoonPhase);
}

void ATimeOfDayHandler::OnRep_DaysPlayed()
{
	SetDaysPlayed(DaysPlayed);
}

void ATimeOfDayHandler::OnRep_NormalizedProgressThroughDay()
{
	SetNormalizedProgressThroughDay(NormalizedProgressThroughDay);
	HandleDelegates();
}

void ATimeOfDayHandler::HandleDelegates()
{
	// Sunrise = 0, Noon = .25, Sunset = .5, Midnight = .75
	const bool SunriseConditionValid = NormalizedProgressThroughDay > 0 && NormalizedProgressThroughDay < 0.05f;
	const bool NoonConditionValid = NormalizedProgressThroughDay > 0.25f && NormalizedProgressThroughDay < 0.3f;
	const bool SunsetConditionValid = NormalizedProgressThroughDay > 0.5f && NormalizedProgressThroughDay < 0.55f;
	const bool MidnightConditionValid = NormalizedProgressThroughDay > 0.75f && NormalizedProgressThroughDay < 0.8f;
	if (!SunriseBroadcasted && OnTimeSunrise.IsBound() && SunriseConditionValid)
	{
		OnTimeSunrise.Broadcast();
		
		SunriseBroadcasted = true;
		NoonBroadcasted = false;
		SunsetBroadcasted = false;
		MidnightBroadcasted = false;
	}
	else if (!NoonBroadcasted && OnTimeNoon.IsBound() && NoonConditionValid)
	{
		OnTimeNoon.Broadcast();

		SunriseBroadcasted = false;
		NoonBroadcasted = true;
		SunsetBroadcasted = false;
		MidnightBroadcasted = false;
	}
	else if (!SunsetConditionValid && OnTimeSunset.IsBound() && SunsetConditionValid)
	{
		OnTimeSunset.Broadcast();

		SunriseBroadcasted = false;
		NoonBroadcasted = false;
		SunsetBroadcasted = true;
		MidnightBroadcasted = false;
	}
	else if (!MidnightConditionValid && OnTimeMidnight.IsBound() && MidnightConditionValid)
	{
		OnTimeMidnight.Broadcast();

		SunriseBroadcasted = false;
		NoonBroadcasted = false;
		SunsetBroadcasted = false;
		MidnightBroadcasted = true;
	}
}

// Called every frame
void ATimeOfDayHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NormalizedProgressThroughDay += (DAY_NIGHT_SPEED * DeltaTime / 360.0f);
	if (NormalizedProgressThroughDay >= 1.0f)
	{
		DaysPlayed += 1;
		NormalizedProgressThroughDay = 0.0f;
	}

	OnRep_DaysPlayed();
	OnRep_NormalizedProgressThroughDay();
}

void ATimeOfDayHandler::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATimeOfDayHandler, DaysPlayed);
	DOREPLIFETIME(ATimeOfDayHandler, NormalizedProgressThroughDay);
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
	if (DirectionalLight == nullptr)
	{
		return;
	}

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
	if (DirectionalLight == nullptr)
	{
		return false;
	}

	return DirectionalLight->GetActorRotation().Pitch < 0.0f;
}

bool ATimeOfDayHandler::IsNight() const
{
	return !IsDay();
}