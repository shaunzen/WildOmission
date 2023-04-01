// Copyright Telephone Studios. All Rights Reserved.


#include "ResourceSaveHandlerComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "WildOmission/Resources/HarvestableResource.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

static UDataTable* BiomeGenerationDataTable = nullptr;

// Sets default values for this component's properties
UResourceSaveHandlerComponent::UResourceSaveHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> BiomeDataTableBlueprint(TEXT("/Game/WildOmission/Core/DataTables/DT_BiomeGenerationData"));
	if (BiomeDataTableBlueprint.Succeeded())
	{
		BiomeGenerationDataTable = BiomeDataTableBlueprint.Object;
	}
}

void UResourceSaveHandlerComponent::Generate(const FWorldGenerationSettings& GenerationSettings)
{
	GenerateTrees(GenerationSettings);
	GenerateNodes(GenerationSettings);
}

void UResourceSaveHandlerComponent::Save(TArray<FHarvestableResourceSave>& OutHarvestableSaves, TArray<FCollectableResourceSave>& OutCollectableSaves)
{
	OutHarvestableSaves.Empty();
	OutCollectableSaves.Empty();

	TArray<AActor*> HarvestableActors;

	//TODO collectables
	TArray<AActor*> CollectableActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHarvestableResource::StaticClass(), HarvestableActors);
	if (HarvestableActors.Num() == 0)
	{
		return;
	}

	for (AActor* HarvestableActor : HarvestableActors)
	{
		AHarvestableResource* Harvestable = Cast<AHarvestableResource>(HarvestableActor);
		if (Harvestable == nullptr)
		{
			return;
		}

		FHarvestableResourceSave Save;

		Save.Class = Harvestable->GetClass();
		Save.Durability = Harvestable->GetDurability();
		Save.Transform = Harvestable->GetActorTransform();

		OutHarvestableSaves.Add(Save);
	}
}

void UResourceSaveHandlerComponent::Load(const TArray<FHarvestableResourceSave>& InHarvestableSaves, const TArray<FCollectableResourceSave>& InCollectableSaves)
{
	for (const FHarvestableResourceSave& Harvestable : InHarvestableSaves)
	{
		AHarvestableResource* SpawnedHarvestable = GetWorld()->SpawnActor<AHarvestableResource>(Harvestable.Class, Harvestable.Transform);
		SpawnedHarvestable->SetDurability(Harvestable.Durability);
	}

	// TODO collectables later
}

FBiomeGenerationData* UResourceSaveHandlerComponent::GetBiomeGenerationData(const FName& BiomeName)
{
	if (BiomeGenerationDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Biome Generation Data Context"));

	return BiomeGenerationDataTable->FindRow<FBiomeGenerationData>(BiomeName, ContextString, true);
}

void UResourceSaveHandlerComponent::GenerateTrees(const FWorldGenerationSettings& GenerationSettings)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Orange, FString("Generating Trees"));
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FHarvestableResourceData& Tree : BiomeData->Trees)
	{
		int32 AmountOfTreeToSpawn = FMath::RoundToInt32((WorldAreaMeters * Tree.DensityPerMeter) / BiomeData->Trees.Num());
		for (int32 i = 0; i < AmountOfTreeToSpawn; i++)
		{
			FVector LocationToSpawn = FVector::ZeroVector;
			FRotator RotationToSpawn = FRotator::ZeroRotator;
			RotationToSpawn.Yaw = FMath::RandRange(0, 360);

			if (!FindSpawnLocation(GenerationSettings, LocationToSpawn))
			{
				continue;
			}

			AHarvestableResource* SpawnedTree = GetWorld()->SpawnActor<AHarvestableResource>(Tree.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
	
}

void UResourceSaveHandlerComponent::GenerateNodes(const FWorldGenerationSettings& GenerationSettings)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Orange, FString("Generating Nodes"));
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

	int32 WorldAreaMeters = GenerationSettings.WorldSizeMetersX * GenerationSettings.WorldSizeMetersY;

	for (const FHarvestableResourceData& Node : BiomeData->Nodes)
	{
		int32 AmountOfNodeToSpawn = FMath::RoundToInt32((WorldAreaMeters * Node.DensityPerMeter) / BiomeData->Nodes.Num());
		for (int32 i = 0; i < AmountOfNodeToSpawn; i++)
		{
			FVector LocationToSpawn = FVector::ZeroVector;
			FRotator RotationToSpawn = FRotator::ZeroRotator;
			RotationToSpawn.Yaw = FMath::RandRange(0, 360);

			if (!FindSpawnLocation(GenerationSettings, LocationToSpawn))
			{
				continue;
			}

			AHarvestableResource* SpawnedNode = GetWorld()->SpawnActor<AHarvestableResource>(Node.BlueprintClass, LocationToSpawn, RotationToSpawn);
		}
	}
}

bool UResourceSaveHandlerComponent::FindSpawnLocation(const FWorldGenerationSettings& GenerationSettings, FVector& OutLocation)
{
	int32 HalfWorldCentimetersX = (GenerationSettings.WorldSizeMetersX * 0.5f) * 100;
	int32 HalfWorldCentimetersY = (GenerationSettings.WorldSizeMetersY * 0.5f) * 100;

	FHitResult HitResult;
	FVector Start = FVector(FMath::RandRange(-HalfWorldCentimetersX, HalfWorldCentimetersX), FMath::RandRange(-HalfWorldCentimetersY, HalfWorldCentimetersY), GenerationSettings.WorldHeightMeters * 100);
	FVector End = Start - FVector(0, 0, GenerationSettings.WorldHeightMeters * 200);

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel2, Params))
	{
		if (HitResult.PhysMaterial == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString::Printf(TEXT("Invalid Surface on actor %s"), *HitResult.GetActor()->GetActorNameOrLabel()));
			return false;
		}
		if (HitResult.PhysMaterial->SurfaceType != SurfaceType1)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString("Invalid Location: Non-Grass Surface"));
			return false;
		}

		OutLocation = HitResult.ImpactPoint;
		return true;
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString("Invalid Location: Nothing was hit"));
	return false;
}