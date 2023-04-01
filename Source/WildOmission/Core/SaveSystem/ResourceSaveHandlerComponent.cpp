// Copyright Telephone Studios. All Rights Reserved.


#include "ResourceSaveHandlerComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"

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

FBiomeGenerationData* UResourceSaveHandlerComponent::GetBiomeGenerationData(const FName& BiomeName)
{
	if (BiomeGenerationDataTable == nullptr)
	{
		return;
	}

	static const FString ContextString(TEXT("Biome Generation Data Context"));

	return BiomeGenerationDataTable->FindRow<FBiomeGenerationData>(BiomeName, ContextString, true);
}

void UResourceSaveHandlerComponent::GenerateTrees(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

}

void UResourceSaveHandlerComponent::GenerateNodes(const FWorldGenerationSettings& GenerationSettings)
{
	const FName DefaultBiome(TEXT("Plains"));
	FBiomeGenerationData* BiomeData = GetBiomeGenerationData(DefaultBiome);
	if (BiomeData == nullptr)
	{
		return;
	}

}

bool UResourceSaveHandlerComponent::FindSpawnLocation(FVector& OutLocation)
{
	return false;
}