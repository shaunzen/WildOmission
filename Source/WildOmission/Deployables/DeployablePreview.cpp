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
	GetStaticMeshComponent()->SetCollisionProfileName(FName("OverlapAll"));
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	
	OnGround = false;
	InvalidOverlap = false;

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
	GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADeployablePreview::OnMeshBeginOverlap);
	GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &ADeployablePreview::OnMeshEndOverlap);
}

void ADeployablePreview::Update(bool IsSpawnValid)
{
	GetStaticMeshComponent()->SetScalarParameterValueOnMaterials(FName("Valid"), IsSpawnValid);
}

bool ADeployablePreview::IsGrounded() const
{
	return OnGround;
}

bool ADeployablePreview::IsOverlappingInvalidObject() const
{
	return InvalidOverlap;
}

void ADeployablePreview::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Ground")))
	{
		OnGround = true;
	}
	else if (OtherComponent->ComponentHasTag(FName("Wall")) || OtherComponent->ComponentHasTag(FName("Foundation")) || OtherComponent->ComponentHasTag(FName("BuildAnchor")))
	{

	}
	else
	{
		InvalidOverlap = true;
	}
}

void ADeployablePreview::OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Ground")))
	{
		OnGround = true;
	}
	else if (OtherComponent->ComponentHasTag(FName("Wall")) || OtherComponent->ComponentHasTag(FName("Foundation")) || OtherComponent->ComponentHasTag(FName("BuildAnchor")))
	{

	}
	else
	{
		InvalidOverlap = false;
	}
}