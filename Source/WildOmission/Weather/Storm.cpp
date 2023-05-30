// Copyright Telephone Studios. All Rights Reserved.


#include "Storm.h"
#include "WeatherManager.h"
#include "Lightning.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AStorm::AStorm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);

	StormRootComponent = CreateDefaultSubobject<USceneComponent>(FName("StormRootComponent"));
	RootComponent = StormRootComponent;

	CloudMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("CloudMeshComponent"));
	CloudMeshComponent->SetupAttachment(StormRootComponent);
	CloudMeshComponent->SetWorldScale3D(FVector(1000.0f , 1000.0f, 50.0f));
	CloudMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Block);

	RainHazeComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("RainHazeComponent"));
	RainHazeComponent->SetupAttachment(StormRootComponent);

	RainSeverityThreshold = 30.0f;
	TornadoSeverityThreshold = 90.0f;

	SpawnedTornado = nullptr;
	HasSpawnedTornado = false;

	Tags.Add(FName("StormCloud"));

	static ConstructorHelpers::FClassFinder<ATornado> TornadoBlueprint(TEXT("/Game/WildOmission/Weather/BP_Tornado"));
	if (TornadoBlueprint.Succeeded())
	{
		TornadoClass = TornadoBlueprint.Class;
	}

	static ConstructorHelpers::FClassFinder<ALightning> LightningBlueprint(TEXT("/Game/WildOmission/Weather/BP_Lightning"));
	if (LightningBlueprint.Succeeded())
	{
		LightningClass = LightningBlueprint.Class;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> RainHazeBlueprint(TEXT("/Game/WildOmission/Art/Weather/NS_RainHaze"));
	if (RainHazeBlueprint.Succeeded())
	{
		RainHazeComponent->SetAsset(RainHazeBlueprint.Object);
	}
}


void AStorm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStorm, Severity);
	DOREPLIFETIME(AStorm, SpawnedTornado);
}

void AStorm::Serialize(FArchive& Ar)
{
	if (Ar.IsSaving())
	{
		if (SpawnedTornado == nullptr)
		{
			TornadoSave = FTornadoSaveInformation();
		}
		else
		{
			TornadoSave = SpawnedTornado->GetSaveInformation();
			TornadoSave.WasSpawned = true;
		}
	}
	Super::Serialize(Ar);
}

// Called when the game starts or when spawned
void AStorm::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));
	}
}

void AStorm::OnSpawn(const FVector2D& InWorldSize)
{
	// Convert to centimeters
	WorldSize = InWorldSize * 100.0f;

	GetSpawnLocation(SpawnLocation);
	SetActorLocation(SpawnLocation);

	TargetLocation = FVector(-SpawnLocation.X, -SpawnLocation.Y, SpawnLocation.Z);
	DistanceToTravel = FVector::Distance(SpawnLocation, TargetLocation);
	DistanceTraveled = 0.0f;
	
	Severity = 0.0f;

	MovementSpeed = FMath::RandRange(300.0f, 1000.0f);
	SeverityMultiplier = (FMath::RandRange(0.0f, 100.0f) / 1000.0f);

	UE_LOG(LogTemp, Display, TEXT("Spawned Storm with MovementSpeed: %f, and SeverityMultiplier: %f"), MovementSpeed, SeverityMultiplier);
}

// Called every frame
void AStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update cloud scale based on severity
	FVector CloudScale = FVector::ZeroVector;
	CloudScale.X = FMath::Lerp(1000.0f, 1500.0f, Severity / 100.0f);
	CloudScale.Y = FMath::Lerp(1000.0f, 1500.0f, Severity / 100.0f);
	CloudScale.Z = FMath::Lerp(50.0f, 700.0f, Severity / 100.0f);
	CloudMeshComponent->SetWorldScale3D(CloudScale);

	CloudMeshComponent->SetCustomPrimitiveDataFloat(0, Severity);

	if (!HasAuthority())
	{
		return;
	}

	HandleMovement();
	HandleSeverity();
	HandleLightning();
}

void AStorm::HandleMovement()
{
	FVector CurrentLocation = GetActorLocation();
	MovementVector = (TargetLocation - CurrentLocation).GetSafeNormal();

	DistanceTraveled = FVector::Distance(CurrentLocation, SpawnLocation);

	FVector NewLocation = CurrentLocation + (MovementVector * MovementSpeed * GetWorld()->GetDeltaSeconds());
	SetActorLocation(NewLocation);

	if (DistanceTraveled >= DistanceToTravel)
	{
		HandleDestruction();
	}
}

void AStorm::HandleSeverity()
{
	// Update severity values
	Severity = FMath::Clamp(Severity + (SeverityMultiplier * GetWorld()->GetDeltaSeconds()), 0.0f, 100.0f);
	
	if (Severity > RainSeverityThreshold && LocalPlayerUnder == false)
	{
		RainHazeComponent->Activate();
	}
	else
	{
		RainHazeComponent->Deactivate();
	}

	if (Severity > TornadoSeverityThreshold && SpawnedTornado == nullptr && HasSpawnedTornado == false)
	{
		SpawnTornado();
	}
}

void AStorm::HandleLightning()
{
	if (NextLightningStrikeTime < KINDA_SMALL_NUMBER)
	{
		NextLightningStrikeTime = FMath::RandRange(1.0f, 30.0f);
		SpawnLightning();
	}
	NextLightningStrikeTime -= GetWorld()->GetDeltaSeconds();
}

void AStorm::SpawnLightning()
{
	if (LightningClass == nullptr)
	{
		return;
	}

	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(true, Origin, BoxExtent);
	float HalfStormRadius = (BoxExtent.Length() - (BoxExtent.Length() * 0.2f) * 0.5f);
	FVector LocationToSpawn = GetActorLocation();
	LocationToSpawn.X = FMath::RandRange(-HalfStormRadius, HalfStormRadius) + GetActorLocation().X;
	LocationToSpawn.Y = FMath::RandRange(-HalfStormRadius, HalfStormRadius) + GetActorLocation().Y;
	
	FRotator RotationToSpawn = FRotator::ZeroRotator;
	RotationToSpawn.Pitch = FMath::RandRange(120.0f, 240.0f);
	RotationToSpawn.Roll = FMath::RandRange(0.0f, 360.0f);
	GetWorld()->SpawnActor<ALightning>(LightningClass, LocationToSpawn, RotationToSpawn);
}

void AStorm::SpawnTornado(bool bFromSave)
{
	SpawnedTornado = GetWorld()->SpawnActor<ATornado>(TornadoClass, FVector(0.0f,0.0f, 999999.0f), FRotator::ZeroRotator);
	SpawnedTornado->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	if (bFromSave)
	{
		SpawnedTornado->LoadSaveInformation(TornadoSave);
		return;
	}

	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(true, Origin, BoxExtent);

	SpawnedTornado->OnSpawn(BoxExtent.Length() - (BoxExtent.Length() * 0.2f));
	HasSpawnedTornado = true;
}

void AStorm::HandleDestruction()
{
	// If there is any tornados, destroy them
	if (SpawnedTornado != nullptr)
	{
		SpawnedTornado->Destroy();
	}

	WeatherManager->RemoveStormFromList(this);
	Destroy();
}

void AStorm::GetSpawnLocation(FVector& OutLocation)
{
	int32 WorldSide = FMath::RandRange(0, 3);
	float StormAltitude = 20000.0f;

	switch (WorldSide)
	{
	case 0: // Top
		OutLocation.Y = WorldSize.Y * 5.0f;
		OutLocation.X = FMath::RandRange(-WorldSize.X, WorldSize.X);
		break;
	case 1: // Bottom
		OutLocation.Y = -(WorldSize.Y * 5.0f);
		OutLocation.X = FMath::RandRange(-WorldSize.X, WorldSize.X);
		break;
	case 2:	// Left
		OutLocation.Y = FMath::RandRange(-WorldSize.Y, WorldSize.Y);
		OutLocation.X = WorldSize.X * 5.0f;
		break;
	case 3: // Right
		OutLocation.Y = FMath::RandRange(-WorldSize.Y, WorldSize.Y);
		OutLocation.X = -(WorldSize.X * 5.0f);
		break;
	}

	OutLocation.Z = StormAltitude;
}

bool AStorm::IsRaining(float& OutDensity) const
{
	OutDensity = FMath::Lerp(100.0f, 2000.0f, Severity / 100.0f);
	return Severity > RainSeverityThreshold;
}

void AStorm::SetLocalPlayerUnderneath(bool IsUnder)
{
	LocalPlayerUnder = IsUnder;
}

void AStorm::SetSeverity(float NewSeverity)
{
	Severity = NewSeverity;
}

float AStorm::GetSeverity() const
{
	return Severity;
}

float AStorm::GetDistanceToTravel() const
{
	return DistanceToTravel;
}

float AStorm::GetDistanceTraveled() const
{
	return DistanceTraveled;
}

FVector AStorm::GetMovementVector() const
{
	return MovementVector;
}

ATornado* AStorm::GetSpawnedTornado() const
{
	return SpawnedTornado;
}

void AStorm::OnLoadComplete_Implementation()
{
	if (WeatherManager == nullptr)
	{
		return;
	}
	
	WeatherManager->SetCurrentStorm(this);

	if (TornadoSave.WasSpawned)
	{
		SpawnTornado(true);
	}
}