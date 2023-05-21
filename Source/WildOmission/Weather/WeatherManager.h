// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherManager.generated.h"

class AStorm;

UCLASS()
class WILDOMISSION_API AWeatherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetNextStormChanceTime() const;
	void SetNextStormChanceTime(float NewTime);

	void SpawnStorm();

protected:
	// Called when the game starts or when spawned
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
