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

	GlobalWindStrength = 0.3f;
	GlobalWindDirection = FVector2D(1.0f, 0.0f);
	TornadoOnGround = 0;
	TornadoLocation = FVector2D::ZeroVector;
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
		HandleWind();
		NextStormChanceTime -= DeltaTime;
		if (NextStormChanceTime < KINDA_SMALL_NUMBER)
		{
			TrySpawnStorm();
			GetNewStormChanceTime();
		}
	}

	SetWindParameters();
}

void AWeatherManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeatherManager, GlobalWindStrength);
	DOREPLIFETIME(AWeatherManager, GlobalWindDirection);
	DOREPLIFETIME(AWeatherManager, TornadoOnGround);
	DOREPLIFETIME(AWeatherManager, TornadoLocation);
}

void AWeatherManager::HandleWind()
{
	if (SpawnedStorms.IsEmpty())
	{
		GlobalWindStrength = 0.3f;
		GlobalWindDirection = FVector2D(1.0f, 0.0f);
		TornadoOnGround = 0.0f;
		TornadoLocation = FVector2D::ZeroVector;
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
	float MaxWindStrengthFromStorm = 3.0f;

	float FadeInOutMultiplier = 1.0f;
	if (CurrentStorm->GetDistanceTraveled() < DistanceFadeRatio)
	{
		FadeInOutMultiplier = CurrentStorm->GetDistanceTraveled() / 1000.0f;
	}
	else if (RemainingDistance < DistanceFadeRatio)
	{
		FadeInOutMultiplier = (1000.0f / RemainingDistance) - 1.0f;
	}

	GlobalWindStrength = FMath::Clamp(MaxWindStrengthFromStorm * NormalizedSeverity * FadeInOutMultiplier, 0.3f, MaxWindStrengthFromStorm);	
	GlobalWindDirection = FVector2D(CurrentStorm->GetMovementVector().X, CurrentStorm->GetMovementVector().Y);
	TornadoOnGround = 0.0f;
	TornadoLocation = FVector2D::ZeroVector;

	if (CurrentStorm->GetSpawnedTornado() != nullptr)
	{
		TornadoLocation = FVector2D(CurrentStorm->GetSpawnedTornado()->GetActorLocation().X, CurrentStorm->GetSpawnedTornado()->GetActorLocation().Y);
		TornadoOnGround = 1.0f;
	}
}

void AWeatherManager::SetWindParameters()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("GlobalWindStrength"), GlobalWindStrength);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("GlobalWindDirection"), FLinearColor(GlobalWindDirection.X, GlobalWindDirection.Y, 0.0f, 1.0f));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoOnGround"), TornadoOnGround);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoLocation"), FLinearColor(TornadoLocation.X, TornadoLocation.Y, 0.0f, 1.0f));
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