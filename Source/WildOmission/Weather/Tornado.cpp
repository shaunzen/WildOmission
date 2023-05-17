// Copyright Telephone Studios. All Rights Reserved.


#include "Tornado.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ATornado::ATornado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	MeshComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));
	RootComponent = MeshComponent;

	RadialSuctionComponent = CreateDefaultSubobject<URadialForceComponent>(FName("RadialSuctionComponent"));
	RadialSuctionComponent->ForceStrength = -999999.0f;
	RadialSuctionComponent->Radius = 4000.0f;
	RadialSuctionComponent->Falloff = ERadialImpulseFalloff::RIF_Linear;
	RadialSuctionComponent->SetupAttachment(MeshComponent);
	RadialSuctionComponent->SetRelativeLocation(FVector(1000.0f, 0.0f, 3000.0f));

	RotationSpeed = 30.0f;
	MovementSpeed = 1000.0f;
}

// Called when the game starts or when spawned
void ATornado::BeginPlay()
{
	Super::BeginPlay();
}

void ATornado::OnSpawn(FVector2D InWorldSize)
{
	// Convert World Size from Meters to Centimeters
	WorldSize = InWorldSize * FVector2D(100.0f);

	TargetLocation = GetRandomLocationInWorld();
	
	SetActorLocation(TargetLocation);
	
	GetNewTargetLocation();
}

// Called every frame
void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		HandleMovement();
	}
	
	HandleRotation();
}

void ATornado::HandleMovement()
{
	FVector CurrentLocation = GetActorLocation();
	FVector VectorTowardTarget = (TargetLocation - CurrentLocation).GetSafeNormal();
	float DistanceFromTarget = FVector::Distance(TargetLocation, CurrentLocation);
	
	if (DistanceFromTarget < 100.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Getting new target location."));
		GetNewTargetLocation();
	}

	FVector NewLocation = CurrentLocation + (VectorTowardTarget * MovementSpeed * GetWorld()->GetDeltaSeconds());
	SetActorLocation(NewLocation);
}

void ATornado::HandleRotation()
{
	float NewYaw = MeshComponent->GetRelativeRotation().Yaw - (RotationSpeed * GetWorld()->GetDeltaSeconds());

	if (NewYaw < -360.0f)
	{
		NewYaw = 0.0f;
	}

	MeshComponent->SetRelativeRotation(FRotator(0.0f, NewYaw, 0.0f));
}

void ATornado::GetNewTargetLocation()
{
	OldTargetLocation = TargetLocation;
	TargetLocation = GetRandomLocationInWorld();
}

FVector ATornado::GetRandomLocationInWorld()
{
	float X, Y;
	X = FMath::RandRange(-WorldSize.X, WorldSize.X);
	Y = FMath::RandRange(-WorldSize.Y, WorldSize.Y);

	return FVector(X, Y, 0.0f);
}