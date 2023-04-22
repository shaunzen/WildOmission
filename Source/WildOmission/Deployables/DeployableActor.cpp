// Copyright Telephone Studios. All Rights Reserved.


#include "DeployableActor.h"

// Sets default values
ADeployableActor::ADeployableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	PlacementType = EDeployableType::GroundOrFloor;
	SnapToBuildAnchor = false;
	UseSurfaceNormal = true;
}

// Called when the game starts or when spawned
void ADeployableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeployableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMesh* ADeployableActor::GetMesh() const
{
	return MeshComponent->GetStaticMesh();
}

TEnumAsByte<EDeployableType> ADeployableActor::GetPlacementType() const
{
	return PlacementType;
}

bool ADeployableActor::SnapsToBuildAnchor() const
{
	return SnapToBuildAnchor;
}

bool ADeployableActor::FollowsSurfaceNormal() const
{
	return UseSurfaceNormal;
}