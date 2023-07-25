// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AnimalSpawnHandlerComponent.generated.h"

class AAnimal;
struct FAnimalSpawnData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANIMALS_API UAnimalSpawnHandlerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAnimalSpawnHandlerComponent();

	static TArray<AAnimal*>* GetSpawnedAnimals();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	FTimerHandle NextSpawnCheckTimerHandler;

	UFUNCTION()
	void CheckSpawnConditions();

	int32 GetNumAnimalsWithinSpawnRadius() const;

	UFUNCTION()
	void SpawnAnimals();

	FTransform GetSpawnTransform() const;

	static FAnimalSpawnData* GetSpawnData(const FName& AnimalName);

};
