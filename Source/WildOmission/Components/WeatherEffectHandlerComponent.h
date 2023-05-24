// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeatherEffectHandlerComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class AStorm;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UWeatherEffectHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeatherEffectHandlerComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* RainParticleSystem;

private:

	UPROPERTY()
	UNiagaraComponent* SpawnedRainComponent;

	UPROPERTY()
	UAudioComponent* SpawnedRainAudioComponent;

	UPROPERTY()
	USoundBase* RainSound;

	UPROPERTY()
	UExponentialHeightFogComponent* Fog;

	UPROPERTY()
	AStorm* PreviouslyHitStorm;

	void EnableRainfallEffects(float RainDensity);
	void DisableRainfallEffects();

};
