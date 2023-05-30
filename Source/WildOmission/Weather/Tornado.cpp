// Copyright Telephone Studios. All Rights Reserved.


#include "Tornado.h"
#include "Storm.h"
#include "WindSuckerComponent.h"
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

	SuctionAnchor = CreateDefaultSubobject<USceneComponent>(FName("SuctionAnchor"));
	SuctionAnchor->SetupAttachment(MeshComponent);
	SuctionAnchor->SetRelativeLocation(FVector(0.0f, 0.0f, 3000.0f));

	CloseSuctionComponent1 = CreateDefaultSubobject<UWindSuckerComponent>(FName("CloseSuctionComponent1"));
	CloseSuctionComponent1->SetupAttachment(SuctionAnchor);
	CloseSuctionComponent1->SetRelativeLocation(FVector(2000.0f, 0.0f, 0.0f));
	
	CloseSuctionComponent2 = CreateDefaultSubobject<UWindSuckerComponent>(FName("CloseSuctionComponent2"));
	CloseSuctionComponent2->SetupAttachment(SuctionAnchor);
	CloseSuctionComponent2->SetRelativeLocation(FVector(-2000.0f, 0.0f, 0.0f));

	CloseSuctionComponent3 = CreateDefaultSubobject<UWindSuckerComponent>(FName("CloseSuctionComponent3"));
	CloseSuctionComponent3->SetupAttachment(SuctionAnchor);
	CloseSuctionComponent3->SetRelativeLocation(FVector(0.0f, 2000.0f, 0.0f));

	CloseSuctionComponent4 = CreateDefaultSubobject<UWindSuckerComponent>(FName("CloseSuctionComponent4"));
	CloseSuctionComponent4->SetupAttachment(SuctionAnchor);
	CloseSuctionComponent4->SetRelativeLocation(FVector(0.0f, -2000.0f, 0.0f));

	FarSuctionComponent= CreateDefaultSubobject<UWindSuckerComponent>(FName("FarSuctionComponent"));
	FarSuctionComponent->SetRadius(20000.0f);
	FarSuctionComponent->SetForceStrength(-99999.0f);
	FarSuctionComponent->SetupAttachment(SuctionAnchor);
	FarSuctionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 3000.0f));

	RotationSpeed = 30.0f;
	MovementSpeed = 1000.0f;
}

// Called when the game starts or when spawned
void ATornado::BeginPlay()
{
	Super::BeginPlay();

	CloseSuctionComponent1->Deactivate();
	CloseSuctionComponent2->Deactivate();
	CloseSuctionComponent3->Deactivate();
	CloseSuctionComponent4->Deactivate();
	FarSuctionComponent->Deactivate();

	if (HasAuthority())
	{
		MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATornado::OnActorOverlapVortex);
	}
}

void ATornado::HandleSpawn(AStorm* InOwnerStorm)
{
	OwnerStorm = InOwnerStorm;

	GetStormRadius();
	SetActorLocation(GetRandomLocationInStorm());
	
	TargetLocation = GetRandomLocationInStorm();

	TotalLifetime = FMath::RandRange(120.0f, 300.0f);
	RemainingLifetime = TotalLifetime;

	CloseSuctionComponent1->Activate();
	CloseSuctionComponent2->Activate();
	CloseSuctionComponent3->Activate();
	CloseSuctionComponent4->Activate();
	FarSuctionComponent->Activate();
}

FTornadoSaveInformation ATornado::GetSaveInformation()
{
	FTornadoSaveInformation NewSave;
	NewSave.Transform = GetActorTransform();
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

	SetActorTransform(InSave.Transform);
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
		TargetLocation = GetRandomLocationInStorm();
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
	FVector WindOrigin = FarSuctionComponent->GetComponentLocation();
	float WindRadius = FarSuctionComponent->GetRadius() * 0.5f;
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