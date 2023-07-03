// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/WeatherHandler.h"
#include "Actors/Storm.h"
#include "Actors/WorldGenerationHandler.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"

static UMaterialParameterCollection* MPC_WindCollection = nullptr;

// Sets default values
AWeatherHandler::AWeatherHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 2.0f;
	
	CurrentStorm = nullptr;
	MinStormSpawnTime = 300.0f;
	MaxStormSpawnTime = 3600.0f;
	NextStormSpawnTime = -1.0f;

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
}

// Called when the game starts or when spawned
void AWeatherHandler::BeginPlay()
{
	Super::BeginPlay();

	if (NextStormSpawnTime == -1.0f)
	{
		NextStormSpawnTime = FMath::RandRange(MinStormSpawnTime, MaxStormSpawnTime);
	}
}

void AWeatherHandler::Setup(AWorldGenerationHandler* InWorldGenerationHandler)
{
	WorldGenerationHandler = InWorldGenerationHandler;
}

// Called every frame
void AWeatherHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority())
	{
		return;
	}

	CalculateWindParameters();

	NextStormSpawnTime -= DeltaTime;

	if (CanSpawnStorm())
	{
		NextStormSpawnTime = FMath::RandRange(MinStormSpawnTime, MaxStormSpawnTime);
		SpawnStorm();
	}
}

AStorm* AWeatherHandler::SpawnStorm(bool FromCommand)
{
	if (CurrentStorm)
	{
		return CurrentStorm;
	}

	CurrentStorm = GetWorld()->SpawnActor<AStorm>(StormClass);
	CurrentStorm->HandleSpawn(FromCommand);

	return CurrentStorm;
}

void AWeatherHandler::ClearStorm()
{
	if (CurrentStorm == nullptr)
	{
		return;
	}

	CurrentStorm->HandleDestruction();
	CurrentStorm = nullptr;
}

bool AWeatherHandler::CanSpawnStorm() const
{
	return NextStormSpawnTime < KINDA_SMALL_NUMBER && CurrentStorm == nullptr;
}

void AWeatherHandler::CalculateWindParameters()
{
	FWindParameters NewParams;

	if (CurrentStorm == nullptr)
	{
		Multicast_UpdateWindParameters(NewParams);
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
	NewParams.TornadoOnGround = 0.0f;
	NewParams.TornadoLocation = FVector2D::ZeroVector;

	if (CurrentStorm->GetSpawnedTornado() != nullptr)
	{
		NewParams.TornadoLocation = FVector2D(CurrentStorm->GetSpawnedTornado()->GetActorLocation().X, CurrentStorm->GetSpawnedTornado()->GetActorLocation().Y);
		NewParams.TornadoOnGround = 1.0f;
	}

	Multicast_UpdateWindParameters(NewParams);
}

void AWeatherHandler::Multicast_UpdateWindParameters_Implementation(const FWindParameters& NewParameters)
{
	WindParameters = NewParameters;

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("GlobalWindStrength"), WindParameters.GlobalWindStrength);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("GlobalWindDirection"), FLinearColor(WindParameters.GlobalWindDirection.X, WindParameters.GlobalWindDirection.Y, 0.0f, 1.0f));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoOnGround"), WindParameters.TornadoOnGround);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoLocation"), FLinearColor(WindParameters.TornadoLocation.X, WindParameters.TornadoLocation.Y, 0.0f, 1.0f));
}

AStorm* AWeatherHandler::GetCurrentStorm() const
{
	return CurrentStorm;
}

void AWeatherHandler::SetCurrentStorm(AStorm* NewCurrentStorm)
{
	if (CurrentStorm)
	{
		CurrentStorm->HandleDestruction();
		CurrentStorm = nullptr;
	}

	CurrentStorm = NewCurrentStorm;
}

float AWeatherHandler::GetNextStormSpawnTime() const
{
	return NextStormSpawnTime;
}

void AWeatherHandler::SetNextStormSpawnTime(float NewSpawnTime)
{
	NextStormSpawnTime = FMath::Clamp(NewSpawnTime, 0.0f, MaxStormSpawnTime);
}

AWorldGenerationHandler* AWeatherHandler::GetWorldGenerationHandler() const
{
	return WorldGenerationHandler;
}
