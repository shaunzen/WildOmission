// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DistanceDespawnComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDespawnConditionMetSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPAWNER_API UDistanceDespawnComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDistanceDespawnComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnDespawnConditionMetSignature OnDespawnConditionMet;

private:	
	UPROPERTY()
	float ConditionCheckCounter;

	UFUNCTION()
	void CheckDespawnConditions();

};
