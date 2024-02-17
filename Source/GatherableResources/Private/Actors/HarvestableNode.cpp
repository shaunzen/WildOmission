// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/HarvestableNode.h"
#include "UObject/ConstructorHelpers.h"

AHarvestableNode::AHarvestableNode()
{
	MeshStages = TArray<UStaticMesh*>();

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageThreeMesh(TEXT("/Game/WildOmissionCore/Art/Environment/SM_Node_Stage_03"));
	if (StageThreeMesh.Succeeded())
	{
		MeshStages.Add(StageThreeMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageTwoMesh(TEXT("/Game/WildOmissionCore/Art/Environment/SM_Node_Stage_02"));
	if (StageTwoMesh.Succeeded())
	{
		MeshStages.Add(StageTwoMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageOneMesh(TEXT("/Game/WildOmissionCore/Art/Environment/SM_Node_Stage_01"));
	if (StageOneMesh.Succeeded())
	{
		MeshStages.Add(StageOneMesh.Object);
	}
}

void AHarvestableNode::OnRep_NormalizedDurability()
{
	Super::OnRep_NormalizedDurability();

	RefreshNodeStage();
}

void AHarvestableNode::RefreshNodeStage()
{
	const int32 NodeStageIndex = FMath::Floor(NormalizedDurability * MeshStages.Num());
	if (!MeshStages.IsValidIndex(NodeStageIndex) || MeshStages[NodeStageIndex] == nullptr)
	{
		return;
	}
	
	MeshComponent->SetStaticMesh(MeshStages[NodeStageIndex]);
}
