// Copyright Telephone Studios. All Rights Reserved.

// TODO clean this up
#include "Actors/Storm.h"
#include "Actors/Tornado.h"
#include "Actors/Lightning.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Log.h"

// Sets default values
AStorm::AStorm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);

	StormRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("StormRootComponent"));
	RootComponent = StormRootComponent;

	CloudMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CloudMeshComponent"));
	CloudMeshComponent->SetupAttachment(StormRootComponent);
	CloudMeshComponent->SetWorldScale3D(FVector(1000.0f , 1000.0f, 50.0f));
	CloudMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Block);

	RainHazeComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RainHazeComponent"));
	RainHazeComponent->SetupAttachment(StormRootComponent);

	RainSeverityThreshold = 30.0f;
	TornadoSeverityThreshold = 90.0f;
	
	SpawnLocation = FVector::ZeroVector;
	MovementVector = FVector::ZeroVector;
	MovementSpeed = 0.0f;
	SeverityMultiplier = 0.0f;
	Severity = 0.0f;
	TornadoData = FTornadoData();

	SpawnedTornado = nullptr;
	HasSpawnedTornado = false;

	WasSpawnedFromCommand = false;
	LocalPlayerUnder = false;
	NextLightningStrikeTime = 0.0f;

	LightningClass = nullptr;
	TornadoClass = nullptr;

	Tags.Add(TEXT("StormCloud"));

	static ConstructorHelpers::FClassFinder<ATornado> TornadoBlueprint(TEXT("/Game/Weather/Actors/BP_Tornado"));
	if (TornadoBlueprint.Succeeded())
	{
		TornadoClass = TornadoBlueprint.Class;
	}
	
	static ConstructorHelpers::FClassFinder<ALightning> LightningBlueprint(TEXT("/Game/Weather/Actors/BP_Lightning"));
	if (LightningBlueprint.Succeeded())
	{
		LightningClass = LightningBlueprint.Class;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> RainHazeBlueprint(TEXT("/Game/Weather/Art/NS_RainHaze"));
	if (RainHazeBlueprint.Succeeded())
	{
		RainHazeComponent->SetAsset(RainHazeBlueprint.Object);
	}
}

void AStorm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStorm, SpawnedTornado);
	DOREPLIFETIME(AStorm, Severity);
	DOREPLIFETIME_CONDITION(AStorm, MovementVector, COND_InitialOnly);
}

// Called every frame
void AStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Client
	UpdateCloudAppearance();

	if (!HasAuthority())
	{
		return;
	}

	// Server
	UpdateLocation();
	UpdateSeverity();
	UpdateLightning();
}

void AStorm::Serialize(FArchive& Ar)
{
	if (Ar.IsSaving())
	{
		if (SpawnedTornado == nullptr)
		{
			TornadoData = FTornadoData();
		}
		else
		{
			TornadoData = SpawnedTornado->GetTornadoData();
			TornadoData.WasSpawned = true;
		}
	}
	Super::Serialize(Ar);
}

void AStorm::OnLoadComplete()
{
	if (!TornadoData.WasSpawned)
	{
		return;
	}
	
	this->SpawnTornado(true);
}

void AStorm::Setup(bool SpawnedFromCommand)
{
	WasSpawnedFromCommand = SpawnedFromCommand;

	GetSpawnData(SpawnLocation, MovementVector);

	SetActorLocation(SpawnLocation);

	Severity = 0.0f;

	MovementSpeed = FMath::RandRange(300.0f, 1000.0f);
	SeverityMultiplier = (FMath::RandRange(0.0f, 100.0f) / 1000.0f);

	UE_LOG(LogWeather, Verbose, TEXT("Spawned Storm with MovementSpeed: %f, and SeverityMultiplier: %f"), MovementSpeed, SeverityMultiplier);
}

void AStorm::Cleanup()
{
	// If there is any tornados, destroy them
	if (SpawnedTornado != nullptr)
	{
		SpawnedTornado->Destroy();
	}

	// Broadcast that we are cleaning up
	if (OnCleanup.IsBound())
	{
		OnCleanup.Broadcast();
	}

	// Destroy this instance
	this->Destroy();
}

void AStorm::GetSpawnData(FVector& OutSpawnLocation, FVector& OutMovementVector) const
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	const float SpawnDistance = 1000000.0f;
	const float StormAltitude = 20000.0f;

	// Get a random player to spawn the storm at
	const int32 PlayerIndex = FMath::RandRange(0, World->GetNumPlayerControllers() - 1);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, PlayerIndex);
	if (PlayerPawn == nullptr)
	{
		return;
	}

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();

	// Random directional vector
	const float DirX = FMath::RandRange(-1, 1);
	const float DirY = FMath::RandRange(-1, 1);

	OutSpawnLocation.X = (-DirX * SpawnDistance) + PlayerLocation.X;
	OutSpawnLocation.Y = (-DirY * SpawnDistance) + PlayerLocation.Y;
	OutSpawnLocation.Z = StormAltitude;

	OutMovementVector = FVector(DirX, DirY, 0.0f);

	return;
}

void AStorm::UpdateCloudAppearance()
{
	if (CloudMeshComponent == nullptr || RainHazeComponent == nullptr)
	{
		return;
	}

	FVector CloudScale = FVector::ZeroVector;
	CloudScale.X = FMath::Lerp(1.0f, 2.0f, Severity / 100.0f);
	CloudScale.Y = FMath::Lerp(1.0f, 2.0f, Severity / 100.0f);
	CloudScale.Z = FMath::Lerp(1.0f, 7.0f, Severity / 100.0f);
	CloudMeshComponent->SetWorldScale3D(CloudScale);

	CloudMeshComponent->SetCustomPrimitiveDataFloat(0, Severity);
	RainHazeComponent->SetActive(Severity > RainSeverityThreshold && LocalPlayerUnder == false);
}

void AStorm::UpdateLocation()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	this->SetActorLocation(this->GetActorLocation() + (MovementVector * MovementSpeed * World->GetDeltaSeconds()));

	if (this->GetDistanceTraveled() >= this->GetTravelDistance())
	{
		this->Cleanup();
	}
}

void AStorm::UpdateSeverity()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	// Update severity values
	Severity = FMath::Clamp(Severity + (SeverityMultiplier * World->GetDeltaSeconds()), 0.0f, 100.0f);

	if (Severity > TornadoSeverityThreshold && SpawnedTornado == nullptr && HasSpawnedTornado == false)
	{
		this->SpawnTornado();
	}
}

void AStorm::SpawnTornado(bool bFromSave)
{
	UWorld* World = GetWorld();
	if (World == nullptr || TornadoClass == nullptr)
	{
		return;
	}

	FActorSpawnParameters TornadoSpawnParams;
	TornadoSpawnParams.Owner = this;
	SpawnedTornado = World->SpawnActor<ATornado>(TornadoClass, FVector(0.0f, 0.0f, 999999.0f), FRotator::ZeroRotator, TornadoSpawnParams);
	if (SpawnedTornado == nullptr)
	{
		return;
	}

	SpawnedTornado->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	if (bFromSave)
	{
		SpawnedTornado->LoadTornadoData(TornadoData, this);
		return;
	}

	SpawnedTornado->Setup(this, WasSpawnedFromCommand);
	HasSpawnedTornado = true;
}

void AStorm::SetSeverity(float NewSeverity)
{
	Severity = NewSeverity;
}

void AStorm::SetMovementVector(const FVector& InMovementVector)
{
	MovementVector = InMovementVector;
}

void AStorm::SetLocalPlayerUnderneath(bool IsUnder)
{
	LocalPlayerUnder = IsUnder;
}

bool AStorm::IsRaining(float& OutDensity) const
{
	OutDensity = FMath::Lerp(100.0f, 2000.0f, Severity / 100.0f);
	return Severity > RainSeverityThreshold;
}

float AStorm::GetSeverity() const
{
	return Severity;
}

FVector AStorm::GetMovementVector() const
{
	return MovementVector;
}

FVector AStorm::GetStormTargetLocation() const
{
	const float MaxTravelDistance = 2000000.0f;
	return SpawnLocation + (MovementVector * MaxTravelDistance);
}

float AStorm::GetTravelDistance() const
{
	return FVector::Distance(SpawnLocation, GetStormTargetLocation());
}

float AStorm::GetDistanceTraveled() const
{
	return FVector::Distance(GetActorLocation(), SpawnLocation);
}

ATornado* AStorm::GetSpawnedTornado() const
{
	return SpawnedTornado;
}

void AStorm::UpdateLightning()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	NextLightningStrikeTime -= World->GetDeltaSeconds();
	if (NextLightningStrikeTime > KINDA_SMALL_NUMBER)
	{
		return;
	}

	NextLightningStrikeTime = FMath::RandRange(1.0f, 30.0f);

	FVector Origin;
	FVector BoxExtent;
	this->GetActorBounds(true, Origin, BoxExtent);

	float HalfStormRadius = (BoxExtent.Length() - (BoxExtent.Length() * 0.2f) * 0.5f);

	const FVector LightningSpawnLocation(
		FMath::RandRange(-HalfStormRadius, HalfStormRadius) + Origin.X,
		FMath::RandRange(-HalfStormRadius, HalfStormRadius) + Origin.Y,
		0.0f);
	
	const FRotator LightningSpawnRotation(
		0.0f,
		FMath::RandRange(120.0f, 240.0f),
		FMath::RandRange(0.0f, 360.0f));

	Multi_SpawnLightning(LightningSpawnLocation, LightningSpawnRotation);
}

void AStorm::Multi_SpawnLightning_Implementation(const FVector& LightningSpawnLocation, const FRotator& LightningSpawnRotation)
{
	UWorld* World = GetWorld();
	if (World == nullptr || LightningClass == nullptr)
	{
		return;
	}

	
	World->SpawnActor<ALightning>(LightningClass, LightningSpawnLocation, LightningSpawnRotation);
}
