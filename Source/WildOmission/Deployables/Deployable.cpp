// Copyright Telephone Studios. All Rights Reserved.


#include "Deployable.h"
#include "BuildAnchorComponent.h"

// Sets default values
ADeployable::ADeployable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	bCanSpawnOnGround = true;
	bCanSpawnOnFloor = false;
	bCanSpawnOnWall = false;
	CanSpawnOnAnchor = EBuildAnchorType::None;
	bFollowsSurfaceNormal = false;
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

bool ADeployable::CanSpawnOnGround() const
{
	return bCanSpawnOnGround;
}

bool ADeployable::CanSpawnOnFloor() const
{
	return bCanSpawnOnFloor;
}

bool ADeployable::CanSpawnOnWall() const
{
	return bCanSpawnOnWall;
}

TEnumAsByte<EBuildAnchorType> ADeployable::CanSpawnOnBuildAnchor() const
{
	return CanSpawnOnAnchor;
}

bool ADeployable::FollowsSurfaceNormal() const
{
	return bFollowsSurfaceNormal;
}