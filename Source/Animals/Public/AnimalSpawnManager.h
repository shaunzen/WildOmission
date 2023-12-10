// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AISpawnManager.h"
#include "AnimalSpawnManager.generated.h"

UCLASS()
class ANIMALS_API AAnimalSpawnManager : public AAISpawnManager
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimalSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
};
