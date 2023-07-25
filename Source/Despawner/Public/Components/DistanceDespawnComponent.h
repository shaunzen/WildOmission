// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DistanceDespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPAWNER_API UDistanceDespawnComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDistanceDespawnComponent();

	float GetDespawnDistance() const;
	void SetDespawnDistance(const float& NewDespawnDistance);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditDefaultsOnly)
	float DespawnDistance;

	UFUNCTION()
	void CheckDespawnConditions();

};
