// Copyright Telephone Studios. All Rights Reserved.


#include "HarvestableSaveHandlerComponent.h"
#include "WildOmission/Components/HarvestableComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UHarvestableSaveHandlerComponent::UHarvestableSaveHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FClassFinder<AActor> Tree01Blueprint(TEXT("/Game/WildOmission/Environment/Trees/BP_Tree_01"));
	ConstructorHelpers::FClassFinder<AActor> Tree02Blueprint(TEXT("/Game/WildOmission/Environment/Trees/BP_Tree_02"));
	ConstructorHelpers::FClassFinder<AActor> Tree03Blueprint(TEXT("/Game/WildOmission/Environment/Trees/BP_Tree_03"));
	ConstructorHelpers::FClassFinder<AActor> StoneNodeBlueprint(TEXT("/Game/WildOmission/Environment/Nodes/BP_StoneNode"));

	if (!Tree01Blueprint.Succeeded() || !Tree02Blueprint.Succeeded() || !Tree03Blueprint.Succeeded() || !StoneNodeBlueprint.Succeeded())
	{
		return;
	}

	Tree01 = Tree01Blueprint.Class;
	Tree02 = Tree02Blueprint.Class;
	Tree03 = Tree03Blueprint.Class;
	StoneNode = StoneNodeBlueprint.Class;
}

void UHarvestableSaveHandlerComponent::Generate(const FWorldGenerationSettings& GenerationSettings)
{
	int32 WorldPerimeter = (GenerationSettings.WorldSizeX + GenerationSettings.WorldSizeY) * 2;

	int32 NumberOfTrees = FMath::RoundToInt32(WorldPerimeter * GenerationSettings.TreeDensity);
	int32 NumberOfStone = FMath::RoundToInt32(WorldPerimeter * GenerationSettings.NodeDensity);
	
	for (int32 i = 0; i < NumberOfTrees; ++i)
	{
		FVector LocationToSpawn;
		FRotator RotationToSpawn;
		RotationToSpawn.Yaw = FMath::RandRange(0, 360);
		int8 TypeToSpawn = FMath::RandRange(0, 2);

		if (!FindSpawnLocation(GenerationSettings, LocationToSpawn))
		{
			continue;
		}
		
		switch(TypeToSpawn)
		{
		case 0:
			GetWorld()->SpawnActor<AActor>(Tree01, LocationToSpawn, RotationToSpawn);
			break;
		case 1:
			GetWorld()->SpawnActor<AActor>(Tree02, LocationToSpawn, RotationToSpawn);
			break;
		case 2:
			GetWorld()->SpawnActor<AActor>(Tree03, LocationToSpawn, RotationToSpawn);
			break;
		}
	}

	for (int32 i = 0; i < NumberOfStone; ++i)
	{
		FVector LocationToSpawn;
		FRotator RotationToSpawn = FRotator::ZeroRotator;
		RotationToSpawn.Yaw = FMath::RandRange(0, 360);

		if (!FindSpawnLocation(GenerationSettings, LocationToSpawn))
		{
			continue;
		}

		GetWorld()->SpawnActor<AActor>(StoneNode, LocationToSpawn, RotationToSpawn);
	}
}

void UHarvestableSaveHandlerComponent::Save(TArray<FHarvestableSave>& OutSaves)
{
	OutSaves.Empty();
	
	TArray<AActor*> HarvestableActors;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Harvestable"), HarvestableActors);
	if (HarvestableActors.Num() == 0)
	{
		return;
	}

	for (AActor* HarvestableActor : HarvestableActors)
	{
		UHarvestableComponent* HarvestableComponent = HarvestableActor->FindComponentByClass<UHarvestableComponent>();
		if (HarvestableComponent == nullptr)
		{
			return;
		}

		FHarvestableSave Save;
		Save.Transform = HarvestableActor->GetTransform();
		Save.Type = HarvestableComponent->GetType();
		Save.Durability = HarvestableComponent->GetDurability();

		OutSaves.Add(Save);
	}
}

void UHarvestableSaveHandlerComponent::Load(const TArray<FHarvestableSave>& InSaves)
{
	for (const FHarvestableSave& Save : InSaves)
	{
		//todo find better way of doing this
		if (Save.Type == FName("TREE01"))
		{
			GetWorld()->SpawnActor<AActor>(Tree01, Save.Transform);
		}
		else if (Save.Type == FName("TREE02"))
		{
			GetWorld()->SpawnActor<AActor>(Tree02, Save.Transform);
		}
		else if (Save.Type == FName("TREE03"))
		{
			GetWorld()->SpawnActor<AActor>(Tree03, Save.Transform);
		}
		else if (Save.Type == FName("STONE_NODE"))
		{
			GetWorld()->SpawnActor<AActor>(StoneNode, Save.Transform);
		}
	}
}

bool UHarvestableSaveHandlerComponent::FindSpawnLocation(const FWorldGenerationSettings& GenerationSettings, FVector& OutLocation)
{
	int32 HalfWorldX = GenerationSettings.WorldSizeX * 0.5f;
	int32 HalfWorldY = GenerationSettings.WorldSizeY * 0.5f;

	FHitResult HitResult;
	FVector Start = FVector(FMath::RandRange(-HalfWorldX, HalfWorldX), FMath::RandRange(-HalfWorldY, HalfWorldY), GenerationSettings.WorldHeight);
	FVector End = Start - FVector(0, 0, 2000);
	FCollisionQueryParams Params;

	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel2, Params))
	{
		if (!HitResult.PhysMaterial.IsValid() || HitResult.PhysMaterial->SurfaceType != SurfaceType1)
		{
			return false;
		}

		OutLocation = HitResult.ImpactPoint;
		return true;
	}

	return false;
}