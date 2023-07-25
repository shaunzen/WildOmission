// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimalSpawnHandlerComponent.generated.h"

struct FAnimalSpawnData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANIMALS_API UAnimalSpawnHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAnimalSpawnHandlerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	FTimerHandle NextSpawnCheckTimerHandler;

	UFUNCTION()
	void CheckSpawnConditions();

	int32 GetNumActorsWithinRange(const TArray<AActor*>& Actors, const float& Distance) const;

	UFUNCTION()
	void SpawnAnimals();

	FTransform GetSpawnTransform() const;

	static FAnimalSpawnData* GetSpawnData(const FName& AnimalName);

};
