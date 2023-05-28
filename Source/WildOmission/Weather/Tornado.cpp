// Copyright Telephone Studios. All Rights Reserved.


#include "Tornado.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "WildOmission/Core/Interfaces/DamagedByWind.h"
#include "WildOmission/Core/Interfaces/InteractsWithTornado.h"

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

	RadialSuctionAnchor = CreateDefaultSubobject<USceneComponent>(FName("RadialSuctionAnchor"));
	RadialSuctionAnchor->SetupAttachment(MeshComponent);
	RadialSuctionAnchor->SetRelativeLocation(FVector(0.0f, 0.0f, 3000.0f));

	RadialSuctionComponent1 = CreateDefaultSubobject<URadialForceComponent>(FName("RadialSuctionComponent1"));
	RadialSuctionComponent1->ForceStrength = -999999.0f;
	RadialSuctionComponent1->Radius = 3000.0f;
	RadialSuctionComponent1->Falloff = ERadialImpulseFalloff::RIF_Linear;
	RadialSuctionComponent1->SetupAttachment(RadialSuctionAnchor);
	RadialSuctionComponent1->SetRelativeLocation(FVector(2000.0f, 0.0f, 0.0f));
	
	RadialSuctionComponent2 = CreateDefaultSubobject<URadialForceComponent>(FName("RadialSuctionComponent2"));
	RadialSuctionComponent2->ForceStrength = -999999.0f;
	RadialSuctionComponent2->Radius = 3000.0f;
	RadialSuctionComponent2->Falloff = ERadialImpulseFalloff::RIF_Linear;
	RadialSuctionComponent2->SetupAttachment(RadialSuctionAnchor);
	RadialSuctionComponent2->SetRelativeLocation(FVector(-2000.0f, 0.0f, 0.0f));

	RadialSuctionComponent3 = CreateDefaultSubobject<URadialForceComponent>(FName("RadialSuctionComponent3"));
	RadialSuctionComponent3->ForceStrength = -999999.0f;
	RadialSuctionComponent3->Radius = 3000.0f;
	RadialSuctionComponent3->Falloff = ERadialImpulseFalloff::RIF_Linear;
	RadialSuctionComponent3->SetupAttachment(RadialSuctionAnchor);
	RadialSuctionComponent3->SetRelativeLocation(FVector(0.0f, 2000.0f, 0.0f));

	RadialSuctionComponent4 = CreateDefaultSubobject<URadialForceComponent>(FName("RadialSuctionComponent4"));
	RadialSuctionComponent4->ForceStrength = -999999.0f;
	RadialSuctionComponent4->Radius = 3000.0f;
	RadialSuctionComponent4->Falloff = ERadialImpulseFalloff::RIF_Linear;
	RadialSuctionComponent4->SetupAttachment(RadialSuctionAnchor);
	RadialSuctionComponent4->SetRelativeLocation(FVector(0.0f, -2000.0f, 0.0f));

	DistanceSuctionComponent = CreateDefaultSubobject<URadialForceComponent>(FName("DistanceSuctionComponent"));
	DistanceSuctionComponent->ForceStrength = -99999.0f;
	DistanceSuctionComponent->Radius = 20000.0f;
	DistanceSuctionComponent->Falloff = ERadialImpulseFalloff::RIF_Linear;
	DistanceSuctionComponent->SetupAttachment(RadialSuctionAnchor);
	DistanceSuctionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 3000.0f));


	RotationSpeed = 30.0f;
	MovementSpeed = 1000.0f;
}

// Called when the game starts or when spawned
void ATornado::BeginPlay()
{
	Super::BeginPlay();

	RadialSuctionComponent1->Deactivate();
	RadialSuctionComponent2->Deactivate();
	RadialSuctionComponent3->Deactivate();
	RadialSuctionComponent4->Deactivate();
	DistanceSuctionComponent->Deactivate();

	if (HasAuthority())
	{
		MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATornado::OnActorOverlapVortex);
	}
}

void ATornado::OnSpawn(const float& InStormRadius)
{
	StormRadius = InStormRadius;

	TargetLocation = GetRandomLocationInStorm();

	SetActorLocation(TargetLocation);
	
	GetNewTargetLocation();

	TotalLifetime = FMath::RandRange(120.0f, 300.0f);
	RemainingLifetime = TotalLifetime;

	RadialSuctionComponent1->Activate();
	RadialSuctionComponent2->Activate();
	RadialSuctionComponent3->Activate();
	RadialSuctionComponent4->Activate();
	DistanceSuctionComponent->Activate();
}

FTornadoSaveInformation ATornado::GetSaveInformation()
{
	FTornadoSaveInformation NewSave;
	NewSave.Transform = GetActorTransform();
	NewSave.MovementSpeed = MovementSpeed;
	NewSave.OldTargetLocation = OldTargetLocation;
	NewSave.TotalLifetime = TotalLifetime;
	NewSave.RemainingLifetime = RemainingLifetime;
	NewSave.RotationSpeed = RotationSpeed;
	NewSave.StormRadius = StormRadius;
	NewSave.TargetLocation = TargetLocation;

	return NewSave;
}

void ATornado::LoadSaveInformation(const FTornadoSaveInformation& InSave)
{
	SetActorTransform(InSave.Transform);
	MovementSpeed = InSave.MovementSpeed;
	OldTargetLocation = InSave.OldTargetLocation;
	TotalLifetime = InSave.TotalLifetime;
	RemainingLifetime = InSave.RemainingLifetime;
	RotationSpeed = InSave.RotationSpeed;
	StormRadius = InSave.StormRadius;
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
	IInteractsWithTornado* InteractsWithActor = Cast<IInteractsWithTornado>(OtherActor);
	if (InteractsWithActor == nullptr)
	{
		return;
	}

	InteractsWithActor->OnContactWithTornado();
}

void ATornado::HandleMovement()
{
	FVector CurrentLocation = FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f);
	FVector VectorTowardTarget = (TargetLocation - CurrentLocation).GetSafeNormal();
	float DistanceFromTarget = FVector::Distance(TargetLocation, CurrentLocation);
	
	if (DistanceFromTarget < 100.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Getting new tornado target location."));
		GetNewTargetLocation();
	}

	FVector NewLocation = CurrentLocation + (VectorTowardTarget * MovementSpeed * GetWorld()->GetDeltaSeconds());
	float TotalTimeAlive = TotalLifetime - RemainingLifetime;

	float ZAxis = 0.0f;
	if (TotalTimeAlive < 5.0f)
	{
		ZAxis = FMath::Lerp(10000.0f, 0.0f, TotalTimeAlive / 5.0f);
	}
	else if (RemainingLifetime < 5.0f)
	{
		ZAxis = FMath::Lerp(0.0f, 10000.0f, (5.0f / RemainingLifetime) - 1.0f);
	}

	NewLocation.Z = ZAxis;

	SetActorLocation(NewLocation);
}

void ATornado::HandleDamage()
{
	TArray<FOverlapResult> Overlaps;
	FVector WindOrigin = DistanceSuctionComponent->GetComponentLocation();
	float WindRadius = DistanceSuctionComponent->Radius * 0.5f;
	FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_WorldDynamic);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	GetWorld()->OverlapMultiByObjectType(Overlaps, WindOrigin, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(WindRadius), Params);

	for (const FOverlapResult& Overlap : Overlaps)
	{
		IDamagedByWind* DamagedByWindActor = Cast<IDamagedByWind>(Overlap.GetActor());
		if (DamagedByWindActor == nullptr)
		{
			continue;
		}

		float DistanceFromOrigin = FVector::Distance(Overlap.GetActor()->GetActorLocation(), WindOrigin);
		float DamageMultiplier = FMath::Clamp((WindRadius / DistanceFromOrigin) - 1.0f, 0.0f, 1.0f);
		
		DamagedByWindActor->ApplyWindDamage(this, DamageMultiplier);
	}
}

void ATornado::HandleRotation()
{
	float NewYaw = MeshComponent->GetRelativeRotation().Yaw - (RotationSpeed * GetWorld()->GetDeltaSeconds());
	float NewSuctionYaw = RadialSuctionAnchor->GetRelativeRotation().Yaw - ((RotationSpeed * 4) * GetWorld()->GetDeltaSeconds());

	if (NewYaw < -360.0f)
	{
		NewYaw = 0.0f;
	}

	if (NewSuctionYaw < -360.0f)
	{
		NewSuctionYaw = 0.0f;
	}

	MeshComponent->SetRelativeRotation(FRotator(0.0f, NewYaw, 0.0f));
	RadialSuctionAnchor->SetRelativeRotation(FRotator(0.0f, NewSuctionYaw, 0.0f));
}

void ATornado::GetNewTargetLocation()
{
	OldTargetLocation = TargetLocation;
	TargetLocation = GetRandomLocationInStorm();
}

FVector ATornado::GetRandomLocationInStorm()
{
	float HalfRadius = StormRadius * 0.5f;
	float X, Y;
	X = FMath::RandRange(-HalfRadius, HalfRadius);
	Y = FMath::RandRange(-HalfRadius, HalfRadius);

	return FVector(X, Y, 0.0f);
}