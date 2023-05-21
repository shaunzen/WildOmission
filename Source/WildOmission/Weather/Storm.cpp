// Copyright Telephone Studios. All Rights Reserved.


#include "Storm.h"

// Sets default values
AStorm::AStorm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StormRootComponent = CreateDefaultSubobject<USceneComponent>(FName("StormRootComponent"));
	RootComponent = StormRootComponent;

	CloudMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("CloudMeshComponent"));
	CloudMeshComponent->SetupAttachment(StormRootComponent);
	CloudMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 2500.0f));
	CloudMeshComponent->SetWorldScale3D(FVector(750.0f , 750.0f, 50.0f));
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
	
	MovementSpeed = 300.0f;

	Severity = 0.0f;
	SeverityMultiplier = 0.0f;
}

// Called every frame
void AStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector VectorTowardTarget = (TargetLocation - CurrentLocation).GetSafeNormal();

	DistanceTraveled = FVector::Distance(CurrentLocation, SpawnLocation);

	FVector NewLocation = CurrentLocation + (VectorTowardTarget * MovementSpeed * GetWorld()->GetDeltaSeconds());
	SetActorLocation(NewLocation);
}

void AStorm::GetSpawnLocation(FVector& OutLocation)
{
	int32 WorldSide = FMath::RandRange(0, 3);
	float StormAltitude = 7000.0f;

	switch (WorldSide)
	{
	case 0: // Top
		OutLocation.Y = WorldSize.Y * 2.0f;
		OutLocation.X = FMath::RandRange(-WorldSize.X, WorldSize.X);
		break;
	case 1: // Bottom
		OutLocation.Y = -(WorldSize.Y * 2.0f);
		OutLocation.X = FMath::RandRange(-WorldSize.X, WorldSize.X);
		break;
	case 2:	// Left
		OutLocation.Y = FMath::RandRange(-WorldSize.Y, WorldSize.Y);
		OutLocation.X = WorldSize.X * 2.0f;
		break;
	case 3: // Right
		OutLocation.Y = FMath::RandRange(-WorldSize.Y, WorldSize.Y);
		OutLocation.X = -(WorldSize.X * 2.0f);
		break;
	}

	OutLocation.Z = StormAltitude;
}