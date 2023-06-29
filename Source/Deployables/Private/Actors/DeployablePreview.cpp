// Copyright Telephone Studios. All Rights Reserved.


#include "DeployablePreview.h"
#include "Deployables/Deployable.h"
#include "Log.h"

static UMaterialInstance* PreviewMaterial = nullptr;

ADeployablePreview::ADeployablePreview()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> PreviewMaterialInstanceBlueprint(TEXT("/Game/WildOmission/Art/Deployables/M_DeployablePreview_Inst"));
	if (PreviewMaterialInstanceBlueprint.Succeeded())
	{
		PreviewMaterial = PreviewMaterialInstanceBlueprint.Object;
	}

	SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetCollisionProfileName(FName("NoCollision"));
	CollisionCheckMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh Component"));
	CollisionCheckMeshComponent->SetupAttachment(GetStaticMeshComponent());
	CollisionCheckMeshComponent->SetHiddenInGame(true);
	CollisionCheckMeshComponent->SetCollisionProfileName(FName("OverlapAll"));
	CollisionCheckMeshComponent->SetGenerateOverlapEvents(true);
	CollisionCheckMeshComponent->SetRelativeScale3D(FVector(0.9f));

	InvalidOverlap = false;
	OverlapCount = 0;

	PreviewingDeployable = nullptr;
}

void ADeployablePreview::Setup(ADeployable* DeployableToPreview)
{
	if (DeployableToPreview == nullptr)
	{
		UE_LOG(LogDeployables, Warning, TEXT("Cannot create preview from a null deployable."));
		return;
	}

	PreviewingDeployable = DeployableToPreview;

	GetStaticMeshComponent()->SetStaticMesh(PreviewingDeployable->GetMesh());
	GetStaticMeshComponent()->SetRelativeTransform(PreviewingDeployable->GetMeshTransform());
	GetStaticMeshComponent()->SetMaterial(0, PreviewMaterial);

	CollisionCheckMeshComponent->SetStaticMesh(PreviewingDeployable->GetMesh());

	CollisionCheckMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeployablePreview::OnMeshBeginOverlap);
	CollisionCheckMeshComponent->OnComponentEndOverlap.AddDynamic(this, &ADeployablePreview::OnMeshEndOverlap);
}

void ADeployablePreview::Update(bool IsSpawnValid)
{
	GetStaticMeshComponent()->SetScalarParameterValueOnMaterials(FName("Valid"), IsSpawnValid);
}

bool ADeployablePreview::IsOverlappingInvalidObject() const
{
	return InvalidOverlap;
}

void ADeployablePreview::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Water")) || (OtherActor->ActorHasTag(FName("Ground")) && PreviewingDeployable->CanSpawnOnGround()) || (OtherActor->ActorHasTag(FName("Wall")) && PreviewingDeployable->CanSpawnOnWall()) || (PreviewingDeployable->CanSpawnOnBuildAnchor() != None && OtherActor->ActorHasTag(FName("BuildingPart"))))
	{
		return;
	}

	OverlapCount++;

	InvalidOverlap = true;
}

void ADeployablePreview::OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Water")) || (OtherActor->ActorHasTag(FName("Ground")) && PreviewingDeployable->CanSpawnOnGround()) || (OtherActor->ActorHasTag(FName("Wall")) && PreviewingDeployable->CanSpawnOnWall()) || (PreviewingDeployable->CanSpawnOnBuildAnchor() != None && OtherActor->ActorHasTag(FName("BuildingPart"))))
	{
		return;
	}

	OverlapCount--;
	
	if (OverlapCount > 0)
	{
		return;
	}

	InvalidOverlap = false;
}