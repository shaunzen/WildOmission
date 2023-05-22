// Copyright Telephone Studios. All Rights Reserved.


#include "Storm.h"
#include "Tornado.h"
#include "Net/UnrealNetwork.h"
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

	RainSeverityThreshold = 30.0f;
	TornadoSeverityThreshold = 90.0f;

	Tags.Add(FName("StormCloud"));

	ConstructorHelpers::FClassFinder<ATornado> TornadoBlueprint(TEXT("/Game/WildOmission/Weather/BP_Tornado"));
	if (TornadoBlueprint.Succeeded())
	{
		TornadoClass = TornadoBlueprint.Class;
	}
}


void AStorm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStorm, Severity);
}

// Called when the game starts or when spawned
void AStorm::BeginPlay()
{
	Super::BeginPlay();
	
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

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0, FColor::Red, FString::Printf(TEXT("MovementSpeed: %f"), MovementSpeed));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0, FColor::Red, FString::Printf(TEXT("SeverityMultiplier: %f"), SeverityMultiplier));

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
}

void AStorm::HandleMovement()
{
	FVector CurrentLocation = GetActorLocation();
	FVector VectorTowardTarget = (TargetLocation - CurrentLocation).GetSafeNormal();

	DistanceTraveled = FVector::Distance(CurrentLocation, SpawnLocation);

	FVector NewLocation = CurrentLocation + (VectorTowardTarget * MovementSpeed * GetWorld()->GetDeltaSeconds());
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

	if (Severity > TornadoSeverityThreshold && SpawnedTornado == nullptr)
	{
		SpawnTornado();
	}
}

void AStorm::SpawnTornado()
{
	SpawnedTornado = GetWorld()->SpawnActor<ATornado>(TornadoClass);
	SpawnedTornado->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(true, Origin, BoxExtent);

	SpawnedTornado->OnSpawn(BoxExtent.Length());
}

void AStorm::HandleDestruction()
{
	// If there is any tornados, destroy them

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

void AStorm::SetSeverity(float NewSeverity)
{
	Severity = NewSeverity;
}