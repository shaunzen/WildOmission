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
	
	if (!HasAuthority())
	{
		return;
	}
	HandleWind();
	NextStormChanceTime -= DeltaTime;
	if (NextStormChanceTime < KINDA_SMALL_NUMBER)
	{
		TrySpawnStorm();
		GetNewStormChanceTime();
	}
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
	float RemainingDistance = DistanceToTravel - DistanceTraveled;
	float DistanceFadeRatio = DistanceToTravel * 0.2f;

	float NormalizedSeverity = Severity / 100.0f;
	float MaxWindStrengthFromStorm = 3.0f;

	float FadeInOutMultiplier = 1.0f;
	if (DistanceTraveled < DistanceFadeRatio)
	{
		FadeInOutMultiplier = DistanceTraveled / 1000.0f;
	}
	else if (RemainingDistance < DistanceFadeRatio)
	{
		FadeInOutMultiplier = (1000.0f / RemainingDistance) - 1.0f;
	}

	float WindStrength = FMath::Clamp(MaxWindStrengthFromStorm * NormalizedSeverity * FadeInOutMultiplier, 0.3f, MaxWindStrengthFromStorm);
	FVector TornadoLocation = FVector::ZeroVector;

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("WindStrength"), WindStrength);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("WindDirection"), FLinearColor(MovementVector.X, MovementVector.Y, 0.0f, 1.0f));
	if (SpawnedTornado)
	{
		TornadoLocation = SpawnedTornado->GetActorLocation();
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoOnGround"), 1.0f);
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoLocation"), FLinearColor(TornadoLocation.X, TornadoLocation.Y, 0.0f, 1.0f));
		return;
	}

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoOnGround"), 0.0f);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_WindCollection, FName("TornadoLocation"), FLinearColor(TornadoLocation.X, TornadoLocation.Y, 0.0f, 0.0f));
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
			SpawnedStorms.Remove(Storm);
			continue;
		}

		Storm->HandleDestruction();
	}
}