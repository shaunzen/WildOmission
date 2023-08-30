// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AISpawnHandler.h"
#include "AnimalSpawnHandler.generated.h"

UCLASS()
class ANIMALS_API AAnimalSpawnHandler : public AAISpawnHandler
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimalSpawnHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
};
