// Copyright Telephone Studios. All Rights Reserved.


#include "Storm.h"

// Sets default values
AStorm::AStorm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CloudMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("CloudMeshComponent"));
	RootComponent = CloudMeshComponent;
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

	FVector StartLocation;
	GetStartLocation(StartLocation);
	SetActorLocation(StartLocation);
}

// Called every frame
void AStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStorm::GetStartLocation(FVector& OutLocation)
{
	int32 WorldSide = FMath::RandRange(0, 3);
	float StormAltitude = 3000.0f;

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