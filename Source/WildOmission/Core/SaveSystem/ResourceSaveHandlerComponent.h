// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Structs/WorldGenerationSettings.h"
#include "WildOmission/Core/Structs/BiomeGenerationData.h"
#include "WildOmissionSaveGame.h"
#include "ResourceSaveHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UResourceSaveHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceSaveHandlerComponent();

	UFUNCTION()
	void Generate(const FWorldGenerationSettings& GenerationSettings);

	void Save(TArray<FHarvestableResourceSave>& OutHarvestableSaves, TArray<FCollectableResourceSave>& OutCollectableSaves);
	
	void Load(const TArray<FHarvestableResourceSave>& InHarvestableSaves, const TArray<FCollectableResourceSave>& InCollectableSaves);

	static FBiomeGenerationData* GetBiomeGenerationData(const FName& BiomeName);

private:
	void GenerateTrees(const FWorldGenerationSettings& GenerationSettings);
	void GenerateNodes(const FWorldGenerationSettings& GenerationSettings);
	void GenerateCollectables(const FWorldGenerationSettings& GenerationSettings);

	bool FindSpawnLocation(const FWorldGenerationSettings& GenerationSettings, FVector& OutLocation);

};
