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

	PlacementType = EDeployableType::GroundOnly;
	BuildAnchorToSnapTo = EBuildAnchorType::None;
	UseSurfaceNormal = true;
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

TEnumAsByte<EDeployableType> ADeployable::GetPlacementType() const
{
	return PlacementType;
}

TEnumAsByte<EBuildAnchorType> ADeployable::SnapsToBuildAnchor() const
{
	return BuildAnchorToSnapTo;
}

bool ADeployable::FollowsSurfaceNormal() const
{
	return UseSurfaceNormal;
}