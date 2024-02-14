// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/Tree.h"
#include "Components/BoxComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

static UNiagaraSystem* DustSystem = nullptr;

ATree::ATree()
{
	bIsStump = false;

	DustBoundsComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DustBoundsComponent"));
	DustBoundsComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DustBoundsComponent->SetupAttachment(MeshComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StumpMeshObject(TEXT("/Game/WildOmissionCore/Art/Environment/SM_Tree_Stump"));
	if (StumpMeshObject.Succeeded())
	{
		StumpMesh = StumpMeshObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DustSystemObject(TEXT("/Game/WildOmissionCore/Art/Effects/NS_BoundsDust"));
	if (DustSystemObject.Succeeded())
	{
		DustSystem = DustSystemObject.Object;
	}
}

void ATree::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATree, bIsStump);
}

void ATree::OnContactWithTornado()
{
	if (bIsStump == true)
	{
		return;
	}
	bIsStump = true;
	FlushNetDormancy();
	UpdateMeshState();
}

void ATree::OnLoadComplete_Implementation()
{
	UpdateMeshState();
}

void ATree::UpdateMeshState()
{
	if (bIsStump == false || StumpMesh == nullptr)
	{
		return;
	}

	MeshComponent->SetStaticMesh(StumpMesh);
}

void ATree::PlayDestructionEffects()
{
	UWorld* World = GetWorld();
	if (World == nullptr || DustSystem == nullptr || DustBoundsComponent == nullptr)
	{
		return;
	}

	FVector Origin = DustBoundsComponent->GetLocalBounds().Origin + GetActorLocation();
	FVector BoxExtent = DustBoundsComponent->GetLocalBounds().BoxExtent;
	int32 BoxSurfaceArea = FMath::RoundToInt32(BoxExtent.X + BoxExtent.Y + BoxExtent.Z);

	UNiagaraComponent* SpawnedDust = UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, DustSystem, Origin, GetActorRotation(), FVector(1.0f), true, false);
	if (SpawnedDust == nullptr)
	{
		return;
	}

	SpawnedDust->SetVectorParameter(TEXT("BoundingBox"), BoxExtent);
	SpawnedDust->SetIntParameter(TEXT("BoundingBoxArea"), BoxSurfaceArea);
	SpawnedDust->Activate(true);
}
