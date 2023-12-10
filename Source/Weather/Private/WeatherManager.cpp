// Copyright Telephone Studios. All Rights Reserved.


#include "WeatherManager.h"
#include "Actors/Storm.h"
#include "SaveManager.h"
#include "WildOmissionSaveGame.h"
#include "TimeOfDayManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Net/UnrealNetwork.h"
#include "Log.h"

static AWeatherManager* Instance = nullptr;
static UMaterialParameterCollection* MPC_WindCollection = nullptr;

// Sets default values
AWeatherManager::AWeatherManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 2.0f;
	
	CurrentStorm = nullptr;
	SunriseStormSpawnChance = 0.1f;
	NoonStormSpawnChance = 0.25f;
	SunsetStormSpawnChance = 0.3f;
	MidnightStormSpawnChance = 0.2f;

	static ConstructorHelpers::FClassFinder<AStorm> StormBlueprint(TEXT("/Game/Weather/Actors/BP_Storm"));
	if (StormBlueprint.Succeeded())
	{
		StormClass = StormBlueprint.Class;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> WindParameterCollectionBlueprint(TEXT("/Game/Weather/MPC_Wind"));
	if (WindParameterCollectionBlueprint.Succeeded())
	{
		MPC_WindCollection = WindParameterCollectionBlueprint.Object;
	}
}

AWeatherManager* AWeatherManager::GetWeatherManager()
{
	return Instance;
}

// Called when the game starts or when spawned
void AWeatherManager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr || World->IsEditorWorld() && IsValid(Instance))
	{
		return;
	}

	Instance = this;

	if (!HasAuthority())
	{
		return;
	}
	
	ATimeOfDayManager* TimeOfDayManager = ATimeOfDayManager::GetTimeOfDayManager();
	if (TimeOfDayManager == nullptr)
	{
		return;
	}

	TimeOfDayManager->OnTimeSunrise.AddDynamic(this, &AWeatherManager::AttemptSunriseStorm);
	TimeOfDayManager->OnTimeNoon.AddDynamic(this, &AWeatherManager::AttemptNoonStorm);
	TimeOfDayManager->OnTimeSunset.AddDynamic(this, &AWeatherManager::AttemptSunsetStorm);
	TimeOfDayManager->OnTimeMidnight.AddDynamic(this, &AWeatherManager::AttemptMidnightStorm);
}

void AWeatherManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Instance = nullptr;
}

void AWeatherManager::AttemptSunriseStorm()
{
	const bool ShouldSpawn = UKismetMathLibrary::RandomBoolWithWeight(SunriseStormSpawnChance);
	if (ShouldSpawn == false)
	{
		return;
	}

	SpawnStorm();
}

void AWeatherManager::AttemptNoonStorm()
{
	const bool ShouldSpawn = UKismetMathLibrary::RandomBoolWithWeight(NoonStormSpawnChance);
	if (ShouldSpawn == false)
	{
		return;
	}

	SpawnStorm();
}

void AWeatherManager::AttemptSunsetStorm()
{
	const bool ShouldSpawn = UKismetMathLibrary::RandomBoolWithWeight(SunsetStormSpawnChance);
	if (ShouldSpawn == false)
	{
		return;
	}

	SpawnStorm();
}

void AWeatherManager::AttemptMidnightStorm()
{
	const bool ShouldSpawn = UKismetMathLibrary::RandomBoolWithWeight(MidnightStormSpawnChance);
	if (ShouldSpawn == false)
	{
		return;
	}

	SpawnStorm();
}

void AWeatherManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeatherManager, CurrentStorm);
}

// Called every frame
void AWeatherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CalculateWindParameters();
}

bool AWeatherManager::IsPeacefulMode() const
{
	ASaveManager* SaveManager = ASaveManager::GetSaveManager();
	if (!IsValid(SaveManager))
	{
		return false;
	}
	
	UWildOmissionSaveGame* SaveFile = SaveManager->GetSaveFile();
	if (SaveFile == nullptr)
	{
		return false;
	}

	return SaveFile->Difficulty == EGameDifficulty::EGD_Peaceful;
}

AStorm* AWeatherManager::SpawnStorm(bool FromCommand)
{
	if (CurrentStorm)
	{
		return CurrentStorm;
	}

	CurrentStorm = GetWorld()->SpawnActor<AStorm>(StormClass);
	CurrentStorm->HandleSpawn(FromCommand);

	return CurrentStorm;
}

void AWeatherManager::ClearStorm()
{
	if (CurrentStorm == nullptr)
	{
		return;
	}

	CurrentStorm->HandleDestruction();
	CurrentStorm = nullptr;
}

bool AWeatherManager::CanSpawnStorm() const
{
	return !IsPeacefulMode() && CurrentStorm == nullptr;
}

void AWeatherManager::CalculateWindParameters()
{
	FWindParameters NewParams;

	if (CurrentStorm == nullptr)
	{
		ApplyWindParameters(NewParams);
		return;
	}

	float RemainingDistance = CurrentStorm->GetTravelDistance() - CurrentStorm->GetTraveledDistance();
	float DistanceFadeRatio = CurrentStorm->GetTravelDistance() * 0.2f;

	float NormalizedSeverity = CurrentStorm->GetSeverity() / 100.0f;

	float FadeInOutMultiplier = 1.0f;
	if (CurrentStorm->GetTraveledDistance() < DistanceFadeRatio)
	{
		FadeInOutMultiplier = CurrentStorm->GetTraveledDistance() / 1000.0f;
	}
	else if (RemainingDistance < DistanceFadeRatio)
	{
		FadeInOutMultiplier = (1000.0f / RemainingDistance) - 1.0f;
	}

	NewParams.GlobalWindStrength = FMath::Clamp(NormalizedSeverity * FadeInOutMultiplier, 0.3f, 1.0f);
	NewParams.GlobalWindDirection = FVector2D(CurrentStorm->GetMovementVector().X, CurrentStorm->GetMovementVector().Y);

	// If the wind direction is zero then trees will vanish from sight
	if (NewParams.GlobalWindDirection.IsNearlyZero(KINDA_SMALL_NUMBER))
	{
		NewParams.GlobalWindDirection = FVector2D(0.5f, 0.5f);
	}

	NewParams.TornadoOnGround = 0.0f;
	NewParams.TornadoLocation = FVector2D::ZeroVector;

	if (CurrentStorm->GetSpawnedTornado() != nullptr)
	{
		NewParams.TornadoLocation = FVector2D(CurrentStorm->GetSpawnedTornado()->GetActorLocation().X, CurrentStorm->GetSpawnedTornado()->GetActorLocation().Y);
		NewParams.TornadoOnGround = 1.0f;
	}

	ApplyWindParameters(NewParams);
}

void AWeatherManager::ApplyWindParameters(const FWindParameters& NewParameters)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, TEXT("GlobalWindStrength"), NewParameters.GlobalWindStrength);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, TEXT("GlobalWindDirection"), FLinearColor(NewParameters.GlobalWindDirection.X, NewParameters.GlobalWindDirection.Y, 0.0f, 1.0f));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, TEXT("TornadoOnGround"), NewParameters.TornadoOnGround);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, TEXT("TornadoLocation"), FLinearColor(NewParameters.TornadoLocation.X, NewParameters.TornadoLocation.Y, 0.0f, 1.0f));
}

AStorm* AWeatherManager::GetCurrentStorm() const
{
	return CurrentStorm;
}

void AWeatherManager::SetCurrentStorm(AStorm* NewCurrentStorm)
{
	if (CurrentStorm)
	{
		CurrentStorm->HandleDestruction();
		CurrentStorm = nullptr;
	}

	CurrentStorm = NewCurrentStorm;
}