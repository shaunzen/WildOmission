// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Structs/WorldGenerationSettings.h"
#include "ResourceSaveHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UResourceSaveHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceSaveHandlerComponent();
	
	void Generate(const FWorldGenerationSettings& GenerationSettings);

	void Save();
	void Load();

private:
	// When Biomes and more level generation related things are implemented
	// it would make more sense to reference a data table for each biome
	TSubclassOf<AActor> Tree01;
	TSubclassOf<AActor> Tree02;
	TSubclassOf<AActor> Tree03;
	TSubclassOf<AActor> StoneNode;

	bool FindSpawnLocation(const FWorldGenerationSettings& GenerationSettings, FVector& OutLocation);
};
