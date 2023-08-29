// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AISpawnHandler.h"
#include "AnimalSpawnHandler.generated.h"

class AAnimal;
struct FAnimalSpawnData;

UCLASS()
class ANIMALS_API AAnimalSpawnHandler : public AAISpawnHandler
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimalSpawnHandler();

	static TArray<AAnimal*>* GetSpawnedAnimals();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FTimerHandle NextSpawnCheckTimerHandle;

	UFUNCTION()
	void CheckSpawnConditionsForAllPlayers();
	void CheckSpawnConditionsForPlayer(APawn* PlayerToCheck);

	int32 GetNumAnimalsWithinRadiusFromLocation(const FVector& TestLocation) const;

	UFUNCTION()
	void SpawnAnimalsInRadiusFromOrigin(const FVector& SpawnOrigin);

	UFUNCTION()
	void RemoveAnimalFromList(AAnimal* AnimalToRemove);

	FTransform GetSpawnTransform(const FVector& SpawnOrigin) const;

	static FAnimalSpawnData* GetSpawnData(const FName& AnimalName);

};
