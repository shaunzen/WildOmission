// Copyright Telephone Studios. All Rights Reserved.


#include "DeployablePreview.h"
#include "Deployable.h"

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
		UE_LOG(LogTemp, Warning, TEXT("Cannot create preview from a null deployable."));
		return;
	}
	
	PreviewingDeployable = DeployableToPreview;

	GetStaticMeshComponent()->SetStaticMesh(PreviewingDeployable->GetMesh());
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
	/*if ((OtherActor->ActorHasTag(FName("Ground")) && !PreviewingDeployable->CanSpawnOnGround())
		|| (OtherActor->ActorHasTag(FName("Wall")) && !PreviewingDeployable->CanSpawnOnWall())
		|| (OtherActor->ActorHasTag(FName("Floor")) && !PreviewingDeployable->CanSpawnOnFloor())
		|| OtherActor != nullptr)
	{
		InvalidOverlap = true;
	}*/

	OverlapCount++;

	InvalidOverlap = true;
}

void ADeployablePreview::OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	OverlapCount--;
	
	if (OverlapCount > 0)
	{
		return;
	}

	InvalidOverlap = false;
}