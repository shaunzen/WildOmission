// Copyright Telephone Studios. All Rights Reserved.


#include "Deployable.h"
#include "BuildAnchorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DamageEvents.h"

// Sets default values
ADeployable::ADeployable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 5.0f;
	NetDormancy = DORM_DormantAll;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Overlap);

	MaxDurability = 100.0f;

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
	CurrentDurability = MaxDurability;
	Client_PlayPlacementSound();
}

void ADeployable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeployable, CurrentDurability);
}

void ADeployable::ApplyWindDamage(AActor* WindCauser, float DamageMultiplier)
{
	float DamageToApply = BaseWindDamage * DamageMultiplier * GetWorld()->GetDeltaSeconds();
	FRadialDamageEvent DamageEvent;
	this->TakeDamage(DamageToApply, DamageEvent, nullptr, WindCauser);
}

float ADeployable::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentDurability -= DamageAmount;

	if (CurrentDurability < KINDA_SMALL_NUMBER)
	{
		this->Destroy();
	}

	FlushNetDormancy();
	return DamageAmount;
}

UStaticMesh* ADeployable::GetMesh() const
{
	return MeshComponent->GetStaticMesh();
}

FTransform ADeployable::GetMeshTransform() const
{
	return MeshComponent->GetRelativeTransform();
}

float ADeployable::GetDurabilityPercentage()
{
	return CurrentDurability / MaxDurability;
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