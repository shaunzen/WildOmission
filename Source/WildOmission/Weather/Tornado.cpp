// Copyright Telephone Studios. All Rights Reserved.


#include "Tornado.h"

// Sets default values
ATornado::ATornado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));

	RotationSpeed = 30.0f;
	MovementSpeed = 400.0f;
}

// Called when the game starts or when spawned
void ATornado::BeginPlay()
{
	Super::BeginPlay();
	
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