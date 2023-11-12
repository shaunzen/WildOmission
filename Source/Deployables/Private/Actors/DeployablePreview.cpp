// Copyright Telephone Studios. All Rights Reserved.


#include "DeployablePreview.h"
#include "Deployables/Deployable.h"
#include "Log.h"

static UMaterialInstance* PreviewMaterial = nullptr;

ADeployablePreview::ADeployablePreview()
{
	SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionCheckMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMeshComponent"));
	CollisionCheckMeshComponent->SetupAttachment(GetStaticMeshComponent());
	CollisionCheckMeshComponent->SetHiddenInGame(true);
	CollisionCheckMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	CollisionCheckMeshComponent->SetGenerateOverlapEvents(true);
	CollisionCheckMeshComponent->SetRelativeScale3D(FVector(0.9f));

	InvalidOverlap = false;
	OverlapCount = 0;

	PreviewingDeployable = nullptr;
	PreviewMaterial = nullptr;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> PreviewMaterialInstanceBlueprint(TEXT("/Game/Deployables/Art/M_DeployablePreview_Inst"));
	if (PreviewMaterialInstanceBlueprint.Succeeded())
	{
		PreviewMaterial = PreviewMaterialInstanceBlueprint.Object;
	}
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
	
	// Set all materials on the mesh to use the preview material
	for (int32 i = 0; i < GetStaticMeshComponent()->GetNumMaterials(); i++)
	{
		GetStaticMeshComponent()->SetMaterial(i, PreviewMaterial);
	}

	CollisionCheckMeshComponent->SetStaticMesh(PreviewingDeployable->GetMesh());

	CollisionCheckMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeployablePreview::OnMeshBeginOverlap);
	CollisionCheckMeshComponent->OnComponentEndOverlap.AddDynamic(this, &ADeployablePreview::OnMeshEndOverlap);
}

void ADeployablePreview::Setup(UStaticMesh* PreviewMesh)
{
	PreviewingDeployable = nullptr;

	GetStaticMeshComponent()->SetStaticMesh(PreviewMesh);
	GetStaticMeshComponent()->SetRelativeTransform(FTransform());

	// Set all materials on the mesh to use the preview material
	for (int32 i = 0; i < GetStaticMeshComponent()->GetNumMaterials(); i++)
	{
		GetStaticMeshComponent()->SetMaterial(i, PreviewMaterial);
	}

	CollisionCheckMeshComponent->SetStaticMesh(PreviewMesh);

	CollisionCheckMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeployablePreview::OnMeshBeginOverlap);
	CollisionCheckMeshComponent->OnComponentEndOverlap.AddDynamic(this, &ADeployablePreview::OnMeshEndOverlap);
}

void ADeployablePreview::Update(bool IsSpawnValid)
{
	GetStaticMeshComponent()->SetScalarParameterValueOnMaterials(TEXT("Valid"), IsSpawnValid);
}

bool ADeployablePreview::IsOverlappingInvalidObject() const
{
	return InvalidOverlap;
}

void ADeployablePreview::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Water")) 
		|| PreviewingDeployable == nullptr
		|| (OtherActor->ActorHasTag(TEXT("Ground")) && PreviewingDeployable->CanSpawnOnGround()) 
		|| (OtherActor->ActorHasTag(TEXT("Wall")) && PreviewingDeployable->CanSpawnOnWall()) 
		|| (OtherActor->ActorHasTag(TEXT("Floor")) && PreviewingDeployable->CanSpawnOnFloor()) 
		|| (OtherActor->ActorHasTag(TEXT("Floor")) && PreviewingDeployable->CanSpawnOnCeiling()) 
		|| (PreviewingDeployable->CanSpawnOnBuildAnchor() != None && (OtherActor->ActorHasTag(TEXT("BuildingPart")) || OtherActor->ActorHasTag(TEXT("Ground")))))
	{
		return;
	}

	OverlapCount++;
	
	InvalidOverlap = true;
}

void ADeployablePreview::OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Water"))
		|| PreviewingDeployable == nullptr
		|| (OtherActor->ActorHasTag(TEXT("Ground")) && PreviewingDeployable->CanSpawnOnGround())
		|| (OtherActor->ActorHasTag(TEXT("Wall")) && PreviewingDeployable->CanSpawnOnWall())
		|| (OtherActor->ActorHasTag(TEXT("Floor")) && PreviewingDeployable->CanSpawnOnFloor())
		|| (OtherActor->ActorHasTag(TEXT("Floor")) && PreviewingDeployable->CanSpawnOnCeiling())
		|| (PreviewingDeployable->CanSpawnOnBuildAnchor() != None && (OtherActor->ActorHasTag(TEXT("BuildingPart")) || OtherActor->ActorHasTag(TEXT("Ground")))))
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