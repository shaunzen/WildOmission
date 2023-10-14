// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/Tornado.h"
#include "Actors/Storm.h"
#include "Components/WindSuckerComponent.h"
#include "Interfaces/DamagedByWind.h"
#include "Interfaces/InteractsWithTornado.h"
#include "Log.h"

// Sets default values
ATornado::ATornado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = MeshComponent;

	SuctionAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("SuctionAnchor"));
	SuctionAnchor->SetupAttachment(MeshComponent);
	SuctionAnchor->SetRelativeLocation(FVector(0.0f, 0.0f, 3000.0f));

	CloseSuctionComponent1 = CreateDefaultSubobject<UWindSuckerComponent>(TEXT("CloseSuctionComponent1"));
	CloseSuctionComponent1->SetupAttachment(SuctionAnchor);
	CloseSuctionComponent1->SetRelativeLocation(FVector(2000.0f, 0.0f, 0.0f));
	
	CloseSuctionComponent2 = CreateDefaultSubobject<UWindSuckerComponent>(TEXT("CloseSuctionComponent2"));
	CloseSuctionComponent2->SetupAttachment(SuctionAnchor);
	CloseSuctionComponent2->SetRelativeLocation(FVector(-2000.0f, 0.0f, 0.0f));

	CloseSuctionComponent3 = CreateDefaultSubobject<UWindSuckerComponent>(TEXT("CloseSuctionComponent3"));
	CloseSuctionComponent3->SetupAttachment(SuctionAnchor);
	CloseSuctionComponent3->SetRelativeLocation(FVector(0.0f, 2000.0f, 0.0f));

	CloseSuctionComponent4 = CreateDefaultSubobject<UWindSuckerComponent>(TEXT("CloseSuctionComponent4"));
	CloseSuctionComponent4->SetupAttachment(SuctionAnchor);
	CloseSuctionComponent4->SetRelativeLocation(FVector(0.0f, -2000.0f, 0.0f));

	FarSuctionComponent = CreateDefaultSubobject<UWindSuckerComponent>(TEXT("FarSuctionComponent"));
	FarSuctionComponent->SetRadius(30000.0f);
	FarSuctionComponent->SetForceStrength(-99999.0f);
	FarSuctionComponent->SetupAttachment(SuctionAnchor);
	FarSuctionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 3000.0f));
	FarSuctionComponent->SetDamagesPawn(true);

	RotationSpeed = 30.0f;
	MovementSpeed = 1000.0f;
}

// Called when the game starts or when spawned
void ATornado::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATornado::OnActorOverlapVortex);
	}
}

void ATornado::HandleSpawn(AStorm* InOwnerStorm, bool SpawnAtWorldOrigin)
{
	OwnerStorm = InOwnerStorm;

	GetStormRadius();
	SetActorRelativeLocation(GetRandomLocationInStorm());
	if (SpawnAtWorldOrigin)
	{
		FVector NewSpawnLocation = FVector::ZeroVector;
		SetActorRelativeLocation(NewSpawnLocation);
	}
	
	TargetLocation = GetRandomLocationInStorm();

	TotalLifetime = FMath::RandRange(120.0f, 300.0f);
	RemainingLifetime = TotalLifetime;

}

FTornadoSaveInformation ATornado::GetSaveInformation()
{
	FTornadoSaveInformation NewSave;
	NewSave.Transform = MeshComponent->GetRelativeTransform();
	NewSave.MovementSpeed = MovementSpeed;
	NewSave.TotalLifetime = TotalLifetime;
	NewSave.RemainingLifetime = RemainingLifetime;
	NewSave.RotationSpeed = RotationSpeed;
	NewSave.TargetLocation = TargetLocation;

	return NewSave;
}

void ATornado::LoadSaveInformation(const FTornadoSaveInformation& InSave, AStorm* InOwnerStorm)
{
	OwnerStorm = InOwnerStorm;

	MeshComponent->SetRelativeTransform(InSave.Transform);
	MovementSpeed = InSave.MovementSpeed;
	TotalLifetime = InSave.TotalLifetime;
	RemainingLifetime = InSave.RemainingLifetime;
	RotationSpeed = InSave.RotationSpeed;
	TargetLocation = InSave.TargetLocation;
}

// Called every frame
void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		HandleMovement();
		HandleDamage();
		RemainingLifetime -= DeltaTime;

		if (RemainingLifetime < KINDA_SMALL_NUMBER)
		{
			Destroy();
		}
	}
	
	HandleRotation();
}

void ATornado::OnActorOverlapVortex(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IInteractsWithTornado* InteractsWithActor = Cast<IInteractsWithTornado>(OtherActor))
	{
		InteractsWithActor->OnContactWithTornado();
	}
}

void ATornado::HandleMovement()
{
	FVector CurrentLocation = FVector(MeshComponent->GetRelativeLocation().X, MeshComponent->GetRelativeLocation().Y, 0.0f);
	FVector VectorTowardTarget = (TargetLocation - CurrentLocation).GetSafeNormal();
	const float GroundLevelZ = -GetOwner()->GetActorLocation().Z;
	float DistanceFromTarget = FVector::Distance(TargetLocation, CurrentLocation);
	
	if (DistanceFromTarget < 100.0f)
	{
		UE_LOG(LogWeather, Verbose, TEXT("Getting New Target Location For Tornado."));
		TargetLocation = GetRandomLocationInStorm();
	}

	FVector NewLocation = CurrentLocation + (VectorTowardTarget * MovementSpeed * GetWorld()->GetDeltaSeconds());
	float TotalTimeAlive = TotalLifetime - RemainingLifetime;

	float ZAxis = GroundLevelZ;
	if (TotalTimeAlive < 5.0f)
	{
		ZAxis = FMath::Lerp(0.0f, GroundLevelZ, TotalTimeAlive / 5.0f);
	}
	else if (RemainingLifetime < 5.0f)
	{
		ZAxis = FMath::Lerp(GroundLevelZ, 0.0f, (5.0f / RemainingLifetime) - 1.0f);
	}

	NewLocation.Z = ZAxis;

	MeshComponent->SetRelativeLocation(NewLocation);
}

void ATornado::HandleDamage()
{
	UE_LOG(LogWeather, Verbose, TEXT("Handling Tornado Damage."));

	TArray<FOverlapResult> Overlaps;
	FVector WindOrigin = FarSuctionComponent->GetComponentLocation();
	float WindRadius = FarSuctionComponent->GetRadius() * 0.5f;
	FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldDynamic);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	GetWorld()->OverlapMultiByObjectType(Overlaps, WindOrigin, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(WindRadius), Params);

	for (const FOverlapResult& Overlap : Overlaps)
	{
		IDamagedByWind* DamagedByWindActor = Cast<IDamagedByWind>(Overlap.GetActor());
		if (DamagedByWindActor == nullptr || !HasLineOfSightTo(Overlap.GetActor()))
		{
			continue;
		}

		float DistanceFromOrigin = FVector::Distance(Overlap.GetActor()->GetActorLocation(), WindOrigin);
		float DamageMultiplier = FMath::Clamp(((DistanceFromOrigin - WindRadius) / WindRadius) * -1, 0.0f, 1.0f);
		DamagedByWindActor->ApplyWindDamage(this, DamageMultiplier);
	}
}

void ATornado::HandleRotation()
{
	float NewYaw = MeshComponent->GetRelativeRotation().Yaw - (RotationSpeed * GetWorld()->GetDeltaSeconds());
	float NewSuctionYaw = SuctionAnchor->GetRelativeRotation().Yaw - ((RotationSpeed * 4) * GetWorld()->GetDeltaSeconds());

	if (NewYaw < -360.0f)
	{
		NewYaw = 0.0f;
	}

	if (NewSuctionYaw < -360.0f)
	{
		NewSuctionYaw = 0.0f;
	}

	MeshComponent->SetRelativeRotation(FRotator(0.0f, NewYaw, 0.0f));
	SuctionAnchor->SetRelativeRotation(FRotator(0.0f, NewSuctionYaw, 0.0f));
}

FVector ATornado::GetRandomLocationInStorm()
{
	float HalfRadius = StormRadius * 0.5f;
	float X, Y;
	X = FMath::RandRange(-HalfRadius, HalfRadius);
	Y = FMath::RandRange(-HalfRadius, HalfRadius);

	return FVector(X, Y, 0.0f);
}

void ATornado::GetStormRadius()
{
	if (OwnerStorm == nullptr)
	{
		return;
	}

	FVector Origin;
	FVector BoxExtent;
	OwnerStorm->GetActorBounds(true, Origin, BoxExtent);
	
	StormRadius = BoxExtent.Length() - (BoxExtent.Length() * 0.2f);
}

bool ATornado::HasLineOfSightTo(AActor* InActor) const
{
	if (InActor == nullptr)
	{
		return false;
	}
	const FVector InActorLocation = InActor->GetActorLocation();
	const FVector CurrentLocation = GetActorLocation();

	FHitResult HitResult;
	const FVector TraceStart = FVector(CurrentLocation.X, CurrentLocation.Y, InActorLocation.Z);
	const FVector TowardComponentVector = (InActorLocation - TraceStart).GetSafeNormal();
	const float DistanceFromComponent = FVector::Distance(TraceStart, InActorLocation);
	const FVector TraceEnd = TraceStart + (TowardComponentVector * DistanceFromComponent);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, Params))
	{
		return false;
	}

	if (HitResult.GetActor() == nullptr)
	{
		return false;
	}

	return HitResult.GetActor() == InActor;
}
