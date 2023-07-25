// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/SavableObject.h"
#include "TimerDespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPAWNER_API UTimerDespawnComponent : public UActorComponent, public ISavableObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimerDespawnComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetTimeRemaining() const;
	void SetDespawnTime(float TimeInSeconds);

private:	
	UPROPERTY(EditDefaultsOnly, SaveGame)
	float DespawnTimeSeconds;

};
