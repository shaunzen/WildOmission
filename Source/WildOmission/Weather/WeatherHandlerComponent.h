// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeatherHandlerComponent.generated.h"

class AStorm;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UWeatherHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeatherHandlerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetNextStormChanceTime() const;
	void SetNextStormChanceTime(float NewTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditDefaultsOnly)
	float MinTimeBetweenStorms;
	UPROPERTY(EditDefaultsOnly)
	float MaxTimeBetweenStorms;

	UPROPERTY(EditAnywhere)
	float NextStormChanceTime;

	UPROPERTY()
	TSubclassOf<AStorm> StormClass;

	void GetNewStormChanceTime();
	void TrySpawnStorm();
};
