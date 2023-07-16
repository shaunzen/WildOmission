// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Deployable.h"
#include "Components/BuildAnchorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DamageEvents.h"

#include "DrawDebugHelpers.h"

static UNiagaraSystem* PlacementDustSystem = nullptr;

// Sets default values
ADeployable::ADeployable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 5.0f;
	NetDormancy = DORM_DormantAll;

	DeployableRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DeployableRootComponent"));
	RootComponent = DeployableRootComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(DeployableRootComponent);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Overlap);

	MaxDurability = 100.0f;

	bCanSpawnOnGround = true;
	bCanSpawnOnFloor = false;
	bCanSpawnOnWall = false;
	CanSpawnOnAnchor = EBuildAnchorType::None;
	bFollowsSurfaceNormal = false;
	bCanRotate = false;

	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultPlacementSound(TEXT("/Game/Deployables/Audio/Deployable_Placement_Cue"));
	if (DefaultPlacementSound.Succeeded())
	{
		PlacementSound = DefaultPlacementSound.Object;
		DestructionSound = DefaultPlacementSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> PlacementDustBlueprint(TEXT("/Game/Deployables/Art/NS_DeployablePlacement"));
	if (PlacementDustBlueprint.Succeeded())
	{
		PlacementDustSystem = PlacementDustBlueprint.Object;
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
	Multicast_PlayPlacementEffects();
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

float ADeployable::GetCurrentDurability()
{
	return CurrentDurability;
}

float ADeployable::GetMaxDurability()
{
	return MaxDurability;
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

void ADeployable::Multicast_PlayPlacementEffects_Implementation()
{
	if (PlacementSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlacementSound, GetActorLocation());
	}
	if (PlacementDustSystem)
	{
		FVector Origin = MeshComponent->GetStaticMesh()->GetBounds().Origin + GetActorLocation();
		FVector BoxExtent = MeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
		int32 BoxSurfaceArea = FMath::RoundToInt32(BoxExtent.X + BoxExtent.Y + BoxExtent.Z);

		UNiagaraComponent* SpawnedDust = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PlacementDustSystem, Origin, GetActorRotation(), FVector(1.0f), true, false);
		if (SpawnedDust == nullptr)
		{
			return;
		}

		SpawnedDust->SetVectorParameter(TEXT("BoundingBox"), BoxExtent);
		SpawnedDust->SetIntParameter(TEXT("BoundingBoxArea"), BoxSurfaceArea);
		SpawnedDust->Activate(true);
	}
}

void ADeployable::Multicast_PlayDestructionSound_Implementation()
{
	if (DestructionSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());
}