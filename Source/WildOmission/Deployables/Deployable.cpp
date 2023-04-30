// Copyright Telephone Studios. All Rights Reserved.


#include "Deployable.h"
#include "BuildAnchorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ADeployable::ADeployable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Overlap);

	bCanSpawnOnGround = true;
	bCanSpawnOnFloor = false;
	bCanSpawnOnWall = false;
	CanSpawnOnAnchor = EBuildAnchorType::None;
	bFollowsSurfaceNormal = false;
	bCanRotate = false;

	ConstructorHelpers::FObjectFinder<USoundBase> DefaultPlacementSound(TEXT("/Game/WildOmission/Deployables/Audio/Deployable_Placement_Cue"));
	if (DefaultPlacementSound.Succeeded())
	{
		PlacementSound = DefaultPlacementSound.Object;
		DestructionSound = DefaultPlacementSound.Object;
	}
}

// Called when the game starts or when spawned
void ADeployable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADeployable::OnSpawn()
{
	Client_PlayPlacementSound();
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

FTransform ADeployable::GetMeshTransform() const
{
	return MeshComponent->GetRelativeTransform();
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

bool ADeployable::CanRotate() const
{
	return bCanRotate;
}

void ADeployable::Client_PlayPlacementSound_Implementation()
{
	if (PlacementSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlacementSound, GetActorLocation());
}

void ADeployable::Client_PlayDestructionSound_Implementation()
{
	if (DestructionSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());
}