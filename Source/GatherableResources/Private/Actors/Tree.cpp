// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/Tree.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

ATree::ATree()
{
	bIsStump = false;

	ConstructorHelpers::FObjectFinder<UStaticMesh> StumpMeshObject(TEXT("/Game/WildOmission/Art/Environment/SM_Tree_Stump"));
	if (StumpMeshObject.Succeeded())
	{
		StumpMesh = StumpMeshObject.Object;
	}
}

void ATree::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATree, bIsStump);
}

void ATree::OnContactWithTornado()
{
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