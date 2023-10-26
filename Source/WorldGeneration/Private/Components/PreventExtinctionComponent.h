// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PreventExtinctionComponent.generated.h"

struct FBiomeGenerationData;
struct FSpawnData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPreventExtinctionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPreventExtinctionComponent();
	
	void PreventExtinction();

private:
	void RunChecksForBiome(FBiomeGenerationData* Biome);
	void SpawnCollectables(const FSpawnData& CollectableData);
		
};
