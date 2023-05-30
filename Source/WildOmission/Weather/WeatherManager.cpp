// Copyright Telephone Studios. All Rights Reserved.


#include "WeatherManager.h"
#include "Storm.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "WildOmission/Core/WildOmissionStatics.h"
#include "Net/UnrealNetwork.h"

static UMaterialParameterCollection* MPC_WindCollection = nullptr;

// Sets default values
AWeatherManager::AWeatherManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	static ConstructorHelpers::FClassFinder<AStorm> StormBlueprint(TEXT("/Game/WildOmission/Weather/BP_Storm"));
	if (StormBlueprint.Succeeded())
	{
		StormClass = StormBlueprint.Class;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> WindParameterCollectionBlueprint(TEXT("/Game/WildOmission/Weather/MPC_Wind"));
	if (WindParameterCollectionBlueprint.Succeeded())
	{
		MPC_WindCollection = WindParameterCollectionBlueprint.Object;
	}

	MinTimeBetweenStorms = 300.0f;
	MaxTimeBetweenStorms = 600.0f;
	NextStormChanceTime = 0.0f;
}

// Called when the game starts or when spawned
void AWeatherManager::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}

	if (NextStormChanceTime == 0.0f)
	{
		GetNewStormChanceTime();
	}
}

// Called every frame
void AWeatherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority())
	{
		CalculateWindParameters();
		NextStormChanceTime -= DeltaTime;
		if (NextStormChanceTime < KINDA_SMALL_NUMBER)
		{
			TrySpawnStorm();
			GetNewStormChanceTime();
		}
	}
}

void AWeatherManager::CalculateWindParameters()
{
	FWindParameters NewParams;

	if (SpawnedStorms.IsEmpty())
	{
		NewParams.GlobalWindStrength = 0.3f;
		NewParams.GlobalWindDirection = FVector2D(1.0f, 0.0f);
		NewParams.TornadoOnGround = 0.0f;
		NewParams.TornadoLocation = FVector2D::ZeroVector;
		return;
	}

	AStorm* CurrentStorm = SpawnedStorms[0];
	if (CurrentStorm == nullptr)
	{
		return;
	}

	float RemainingDistance = CurrentStorm->GetDistanceToTravel() - CurrentStorm->GetDistanceTraveled();
	float DistanceFadeRatio = CurrentStorm->GetDistanceToTravel() * 0.2f;

	float NormalizedSeverity = CurrentStorm->GetSeverity() / 100.0f;
	
	float FadeInOutMultiplier = 1.0f;
	if (CurrentStorm->GetDistanceTraveled() < DistanceFadeRatio)
	{
		FadeInOutMultiplier = CurrentStorm->GetDistanceTraveled() / 1000.0f;
	}
	else if (RemainingDistance < DistanceFadeRatio)
	{
		FadeInOutMultiplier = (1000.0f / RemainingDistance) - 1.0f;
	}

	NewParams.GlobalWindStrength = FMath::Clamp(NormalizedSeverity * FadeInOutMultiplier, 0.3f, 1.0f);
	NewParams.GlobalWindDirection = FVector2D(CurrentStorm->GetMovementVector().X, CurrentStorm->GetMovementVector().Y);
	NewParams.TornadoOnGround = 0.0f;
	NewParams.TornadoLocation = FVector2D::ZeroVector;

	if (CurrentStorm->GetSpawnedTornado() != nullptr)
	{
		NewParams.TornadoLocation = FVector2D(CurrentStorm->GetSpawnedTornado()->GetActorLocation().X, CurrentStorm->GetSpawnedTornado()->GetActorLocation().Y);
		NewParams.TornadoOnGround = 1.0f;
	}

	Client_UpdateWindParameters(NewParams);
}

void AWeatherManager::Client_UpdateWindParameters_Implementation(const FWindParameters& NewParameters)
{
	WindParameters = NewParameters;
	//UE_LOG(LogTemp, Warning, TEXT("Wind Settings(GlobalWindStrength: %f, GlobalWindDirection: %s, TornadoOnGround: %f, TornadoLocation: %s"), GlobalWindStrength, *GlobalWindDirection.ToString(), TornadoOnGround, *TornadoLocation.ToString());
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("GlobalWindStrength"), WindParameters.GlobalWindStrength);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("GlobalWindDirection"), FLinearColor(WindParameters.GlobalWindDirection.X, WindParameters.GlobalWindDirection.Y, 0.0f, 1.0f));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoOnGround"), WindParameters.TornadoOnGround);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoLocation"), FLinearColor(WindParameters.TornadoLocation.X, WindParameters.TornadoLocation.Y, 0.0f, 1.0f));
}

void AWeatherManager::RemoveStormFromList(AStorm* StormToRemove)
{
	if (StormToRemove == nullptr)
	{
		return;
	}
	
	SpawnedStorms.Remove(StormToRemove);
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

	SpawnedStorms.Add(SpawnedStorm);
	
	return SpawnedStorm;
}

void AWeatherManager::ClearAllStorms()
{
	for (AStorm* Storm: SpawnedStorms)
	{
		if (Storm == nullptr)
		{
			continue;
		}

		Storm->HandleDestruction();
	}
}