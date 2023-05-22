// Copyright Telephone Studios. All Rights Reserved.


#include "Tree.h"
#include "UObject/ConstructorHelpers.h"

ATree::ATree()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> StumpMeshObject(TEXT("/Game/WildOmission/Art/Environment/Foliage/Trees/SM_Tree_Stump"));
	if (StumpMeshObject.Succeeded())
	{
		StumpMesh = StumpMeshObject.Object;
	}
}

void ATree::OnContactWithTornado()
{
	bIsStump = true;
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