// Copyright Telephone Studios. All Rights Reserved.


#include "Deployable.h"

// Sets default values
ADeployable::ADeployable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	bCanBePlacedOnGround = true;
	bCanBePlacedOnFloor = true;
	bCanBePlacedOnWall = false;
	bCanBePlacedInDoorway = false;
}

// Called when the game starts or when spawned
void ADeployable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeployable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMesh* ADeployable::GetMesh() const
{
	return MeshComponent->GetStaticMesh();
}

bool ADeployable::CanBePlacedOnGround() const
{
	return bCanBePlacedOnGround;
}

bool ADeployable::CanBePlacedOnFloor() const
{
	return bCanBePlacedOnFloor;
}

bool ADeployable::CanBePlacedOnWall() const
{
	return bCanBePlacedOnWall;
}

bool ADeployable::CanBePlacedInDoorway() const
{
	return bCanBePlacedInDoorway;
}