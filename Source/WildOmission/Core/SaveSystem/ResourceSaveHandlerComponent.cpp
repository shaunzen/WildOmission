// Copyright Telephone Studios. All Rights Reserved.


#include "ResourceSaveHandlerComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UResourceSaveHandlerComponent::UResourceSaveHandlerComponent()
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

void UResourceSaveHandlerComponent::Generate()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Generating New Resources."));
	int32 NumberOfTrees = 100;
	int32 NumberOfStone = 100;
	
	for (int32 i = 0; i < NumberOfTrees; ++i)
	{
		bool TreeSpawnFound = false;
		FVector LocationToSpawn;
		int8 TypeToSpawn = FMath::RandRange(0, 2);

		while (TreeSpawnFound == false)
		{
			TreeSpawnFound = FindSpawnLocation(LocationToSpawn);
		}
		//spawn it at location
		switch(TypeToSpawn)
		{
		case 0:
			GetWorld()->SpawnActor<AActor>(Tree01->StaticClass(), LocationToSpawn, FRotator::ZeroRotator);
			break;
		case 1:
			GetWorld()->SpawnActor<AActor>(Tree02->StaticClass(), LocationToSpawn, FRotator::ZeroRotator);
			break;
		case 2:
			GetWorld()->SpawnActor<AActor>(Tree03->StaticClass(), LocationToSpawn, FRotator::ZeroRotator);
			break;
		}
	}

	for (int32 i = 0; i < NumberOfStone; ++i)
	{
		bool FoundSpawn = false;
		FVector LocationToSpawn;
		
		while (FoundSpawn == false)
		{
			FoundSpawn = FindSpawnLocation(LocationToSpawn);
		}

		GetWorld()->SpawnActor<AActor>(StoneNode, LocationToSpawn, FRotator::ZeroRotator);
	}
}

void UResourceSaveHandlerComponent::Save()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Saving Resources."));
}

void UResourceSaveHandlerComponent::Load()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Loading Resources from save."));
}

bool UResourceSaveHandlerComponent::FindSpawnLocation(FVector& OutLocation)
{
	FHitResult HitResult;
	FVector Start = FVector(FMath::RandRange(-10000, 10000), FMath::RandRange(-10000, 10000), 1000);
	FVector End = Start - FVector(0, 0, 2000);
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel2))
	{
		if (HitResult.PhysMaterial == nullptr || HitResult.PhysMaterial->SurfaceType != SurfaceType1)
		{
			return false;
		}

		return true;
	}

	return false;
}