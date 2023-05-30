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

	MinStormSpawnTime = 300.0f;
	MaxStormSpawnTime= 600.0f;
	NextStormSpawnTime = 0.0f;
}

// Called when the game starts or when spawned
void AWeatherManager::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}

	if (NextStormSpawnTime == 0.0f)
	{

	}
}

// Called every frame
void AWeatherManager::Tick(float DeltaTime)
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

AStorm* AWeatherManager::SpawnStorm()
{
	if (CurrentStorm)
	{
		return;
	}

	FVector2D WorldSize;

	UWildOmissionStatics::GetWorldSize(GetWorld(), WorldSize);

	CurrentStorm = GetWorld()->SpawnActor<AStorm>(StormClass);
	CurrentStorm->OnSpawn(WorldSize);

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
	return NextStormSpawnTime < KINDA_SMALL_NUMBER && CurrentStorm == nullptr;
}

void AWeatherManager::CalculateWindParameters()
{
	FWindParameters NewParams;

	if (CurrentStorm == nullptr)
	{
		Client_UpdateWindParameters(NewParams);
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

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("GlobalWindStrength"), WindParameters.GlobalWindStrength);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("GlobalWindDirection"), FLinearColor(WindParameters.GlobalWindDirection.X, WindParameters.GlobalWindDirection.Y, 0.0f, 1.0f));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoOnGround"), WindParameters.TornadoOnGround);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoLocation"), FLinearColor(WindParameters.TornadoLocation.X, WindParameters.TornadoLocation.Y, 0.0f, 1.0f));
}